#include <EEPROM.h>

//  Si5351 Lib from Etherkit
#include <si5351.h>

//  OLED lib from olikraus
#include <U8g2lib.h>
#include <U8x8lib.h>

//  homebrew libs
#include <button.h>
#include <rotencoder.h>

//  Buttons---------------------------------
Button btnMode(5);
Button btnStep(4);

//  RotateEncoder--------------------------
RotateEncoder re(2,3);

//  OLED-----------------------------
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, 
  /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

//  5351------------------------------
Si5351 si5351;

const unsigned int MAGIC_CODE = 0xBA5A;

struct Parameter {
  unsigned long spot;
  unsigned long begin;
  unsigned long end;
  unsigned long sweepstep;  
};

//int mode = 0;
//unsigned long step = 1000;
//unsigned long freq = 14000000;
//unsigned long spot = 14000000;
//unsigned long begin = 14000000;
//unsigned long end = 14000000;
//unsigned long sweepstep = 1000;
static Parameter para;

void setup() {
  Serial.begin(9600);

  u8g2.begin(); 
  
  boolean i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if ( !i2c_found ) {
    showPrompt("NO 5351");
    for ( ;; );
  }

  showPrompt("SG-1");

  si5351.output_enable(SI5351_CLK0, 0);
  
  para = readParameter();
}

Parameter readParameter()
{
  unsigned int mc;
  Parameter p;
  EEPROM.get(0, mc);
  if ( mc == MAGIC_CODE ) {
    EEPROM.get(4, p);
    Serial.println("para loaded");
  } else {
    p.spot = 14000000UL;
    p.begin = 14000000UL;
    p.end = 14350000UL;
    p.sweepstep = 1000;
    Serial.println("para created");
  }
  return p;
}

void writeParameter(const Parameter& p)
{
  EEPROM.put(0, MAGIC_CODE);
  EEPROM.put(4, p);
  Serial.println("para saved");
}

void loop() {
  static const char modeName[][6] = {
    "SG-1","SPOT","BEGIN","END","STEP","SWEEP"
  };
  static unsigned long step = 1000;
  static unsigned long freq = 14000000;
  static unsigned long lasttime = 0;
  static int mode = 0;
  static boolean isModified = false;
  boolean update = false;
  
  if ( btnMode.isPressed() ) {
    boolean switchfreq = false;
    mode = mode+1;
    if ( mode == 1 ) { //  SPOT
      freq = para.spot;
      switchfreq = true;
      si5351.output_enable(SI5351_CLK0, 1);
      lasttime = millis();
    } else if ( mode == 2 ) { //  BEGIN
      para.spot = freq;
      freq = para.begin;
      switchfreq = true;
    } else if ( mode == 3 ) { //  END
      para.begin = freq;
      freq = para.end;
      switchfreq = true;
    } else if ( mode == 4 ) { //  SWEEP STEP
      para.end = freq;
      freq = para.sweepstep;
      step = 1000;
      setrot(freq, step);
      si5351.output_enable(SI5351_CLK0, 0);
    } else if ( mode == 5 ) { //  SWEEP
      para.sweepstep = freq;
      freq = para.begin;
      si5351.output_enable(SI5351_CLK0, 1);
      lasttime = millis();
    } else if ( mode == 6 ) {  //  IDLE
      mode = 0;
      si5351.output_enable(SI5351_CLK0, 0);
    } 
    Serial.print("Mode:");
    Serial.println(mode);
    update = true;
    if ( switchfreq ) {
      setrot(freq, step);
      setfreq(freq);
    }
  }
  if ( mode == 5 ) { //  sweep
    setfreq(freq);
    freq += para.sweepstep;
    if ( freq > para.end ) {
      freq = para.begin;
    }
  } else {
    if ( re.getCount() != freq ) {
      freq = re.getCount();
      Serial.print("Freq:");
      Serial.println(freq);
      setfreq(freq);
      update = true;
      if ( mode >=1 && mode <=4 ) { // spot
        lasttime = millis();
        isModified = true;
      }
    }
    if ( btnStep.isPressed() ) {
      step = step*10;
      if ( step == 1000000000UL ) {
        step = 1;
      }
      Serial.print("Step:");
      Serial.println(step);
      re.setStep(step);
      update = true;
    }
  }
  if ( isModified && (mode == 1 || mode ==5) && millis() - lasttime > 5000 ) {
    writeParameter(para);
    isModified = false;
  } 
  if ( update ) {
    Serial.println(modeName[mode]);
    if ( mode ==0 ) {
      showPrompt(modeName[mode]);
    } else if ( mode == 5 ) { // sweep
      showSweep(modeName[mode], para.begin, para.end);
    } else {
      showFreq(modeName[mode], freq, step);
    }
  }
}

void showPrompt(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB18_tr);
    u8g2.drawStr(32,50,s);
  } while ( u8g2.nextPage() );
}

void ulong2str(unsigned long number, char* buf)
{
  int loc = 8;
  do {
    buf[loc--] = '0'+number%10;
    number /= 10;
  } while(number>0);
}

void showFreq(const char* mode, unsigned long freq, unsigned long position)
{
  char buf[] = "000000000";
  ulong2str(freq, buf);
  int pos = 9;
  do {
    pos --;
    position /= 10;
  } while(position>0);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenR10_tr);
    u8g2.drawStr(32, 43, mode);
    u8g2.setFont(u8g2_font_ncenR10_tr);
    char c = buf[6];
    buf[6] = 0;
    u8g2.drawStr(32, 58, buf);
    u8g2.setFont(u8g2_font_ncenR08_tr);
    buf[6] = c;
    u8g2.drawStr(81, 58, buf+6);
    if ( pos < 6 ) {
      u8g2.drawLine(32+pos*8,59,32+pos*8+5,59);
    } else {
      u8g2.drawLine(81+(pos-6)*5,59,80+(pos-6)*5+3,59);
    }
  } while ( u8g2.nextPage() );
}

void showSweep(const char* mode, unsigned long begin, unsigned long end)
{
  char bufb[] = "000000000";
  char bufe[] = "000000000";
  ulong2str(begin, bufb);
  ulong2str(end, bufe);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenR10_tr);
    u8g2.drawStr(32, 43, mode);
    u8g2.setFont(u8g2_font_ncenR08_tr);
    u8g2.drawStr(32, 55, bufb);
    u8g2.drawStr(32, 64, bufe);
  } while ( u8g2.nextPage() );
}

void setrot(unsigned long freq, unsigned long step)
{
  re.setCount(freq);
  re.setStep(step);
}

void setfreq(unsigned long freq)
{
  si5351.set_freq(freq*100ULL, SI5351_CLK0);
  si5351.update_status();
}