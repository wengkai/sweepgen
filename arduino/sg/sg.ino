//  Si5351 Lib from Etherkit
#include <si5351.h>

//  OLED lib from olikraus
#include <U8g2lib.h>
#include <U8x8lib.h>

//  homebrew libs
#include <button.h>
#include <rotencoder.h>

int mode = 0;
unsigned long step = 1000;
unsigned long freq=14000000;

//  Button---------------------------------

Button btnMode(5);
Button btnStep(4);

//  RotateEncoder--------------------------

RotateEncoder re(2,3);

//  OLED-----------------------------
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

//  5351------------------------------
Si5351 si5351;

void showPrompt(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB18_tr);
    u8g2.drawStr(32,50,s);
  } while ( u8g2.nextPage() );
}

void showFreq(const char* mode, unsigned long freq, unsigned long position)
{
  char buf[] = "000000000";
  int loc = 8;
  do {
    buf[loc--] = '0'+freq%10;
    freq /= 10;
  } while(freq>0);
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

void setup() {
  Serial.begin(9600);

  re.setCount(freq);
  re.setStep(step);

  u8g2.begin(); 
  showPrompt("SG-1");

  boolean i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    Serial.println("Device not found on I2C bus!");
  }

  // Set CLK0 to output 14 MHz
  si5351.set_freq(freq*100ULL, SI5351_CLK0);
  si5351.update_status();
}

void loop() {
  boolean update = false;
  if ( btnMode.isPressed() ) {
    mode = mode+1;
    if (mode==5) mode = 0;
    Serial.print("Mode:");
    Serial.println(mode);
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
  if ( re.getCount() != freq ) {
    freq = re.getCount();
    Serial.print("Freq:");
    Serial.println(freq);
    si5351.set_freq(freq*100ULL, SI5351_CLK0);
    si5351.update_status();
    update = true;
  }
  if ( update ) {
    showFreq("SPOT", freq, step);
  }
}