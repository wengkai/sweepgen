#include <Adafruit_SI5351.h>
#include <asserts.h>
#include <errors.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#include <button.h>
#include <rotencoder.h>

//  Button---------------------------------

Button btnMode(5);
Button btnStep(4);

//  RotateEncoder--------------------------

RotateEncoder re(2,3);

//  OLED-----------------------------
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

//  5351------------------------------
Adafruit_SI5351 clockgen = Adafruit_SI5351();

void showPrompt(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB18_tr);
    u8g2.drawStr(32,50,s);
    // Serial.println("p");
  } while ( u8g2.nextPage() );
  // u8g2.clearBuffer();					// clear the internal memory
  // u8g2.setFont(u8g2_font_ncenB14_tr);	// choose a suitable font
  // u8g2.drawStr(32,50,"hello");	// write something to the internal memory
  // u8g2.sendBuffer();					// transfer internal memory to the display
}

void showFreq(const char* mode, unsigned long freq, int position)
{
  char buf[] = "000,000,000Hz";
  int loc = 10;
  do {
    buf[loc--] = '0'+freq%10;
    freq /= 10;
    if ( loc==3||loc==7 ) loc--;
  } while(freq>0);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenR10_tr);
    u8g2.drawStr(32, 43, mode);
    u8g2.setFont(u8g2_font_ncenR08_tr);
    u8g2.drawStr(32, 58, buf);
  } while ( u8g2.nextPage() );
}

void setup() {
  Serial.begin(9600);
  u8g2.begin(); 
  if (clockgen.begin() != ERROR_NONE)
  {
    /* There was a problem detecting the IC ... check your connections */
    Serial.print("Ooops, no Si5351 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  Serial.println("OK!");
  showPrompt("SG-1");
}

int mode = 0;
int step = 1;
int freq=0;

void loop() {
  if ( btnMode.isPressed() ) {
    mode = mode+1;
    if (mode==5) mode = 0;
    Serial.print("Mode:");
    Serial.println(mode);
  }
  if ( btnStep.isPressed() ) {
    step = step*10;
    if ( step == 1000000 )step = 1;
    Serial.print("Step:");
    Serial.println(step);  
  }
  if ( re.getCount() != freq ) {
    freq = re.getCount();
    Serial.print("Freq:");
    Serial.println(freq);
    showFreq("SPOT", freq, 0);
  }
  
}