#include <si5351.h>

// #include <Adafruit_SI5351.h>
// #include <asserts.h>
// #include <errors.h>

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
// Adafruit_SI5351 clockgen = Adafruit_SI5351();

void showPrompt(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB18_tr);
    u8g2.drawStr(32,50,s);
  } while ( u8g2.nextPage() );
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

Si5351 si5351;

void setup() {
  Serial.begin(9600);
  u8g2.begin(); 
  // if (clockgen.begin() != ERROR_NONE)
  // {
  //   /* There was a problem detecting the IC ... check your connections */
  //   Serial.print("Ooops, no Si5351 detected ... Check your wiring or I2C ADDR!");
  //   while(1);
  // }
  // Serial.println("OK!");
  // /* INTEGER ONLY MODE --> most accurate output */  
  // /* Setup PLLA to integer only mode @ 900MHz (must be 600..900MHz) */
  // /* Set Multisynth 0 to 112.5MHz using integer only mode (div by 4/6/8) */
  // /* 25MHz * 36 = 900 MHz, then 900 MHz / 8 = 112.5 MHz */
  // Serial.println("Set PLLA to 900MHz");
  // clockgen.setupPLLInt(SI5351_PLL_A, 36);
  // Serial.println("Set Output #0 to 112.5MHz");  
  // clockgen.setupMultisynthInt(0, SI5351_PLL_A, SI5351_MULTISYNTH_DIV_8);

  // /* FRACTIONAL MODE --> More flexible but introduce clock jitter */
  // /* Setup PLLB to fractional mode @616.66667MHz (XTAL * 24 + 2/3) */
  // /* Setup Multisynth 1 to 13.55311MHz (PLLB/45.5) */
  // clockgen.setupPLL(SI5351_PLL_B, 24, 2, 3);
  // Serial.println("Set Output #1 to 13.553115MHz");  
  // clockgen.setupMultisynth(1, SI5351_PLL_B, 45, 1, 2);

  // /* Multisynth 2 is not yet used and won't be enabled, but can be */
  // /* Use PLLB @ 616.66667MHz, then divide by 900 -> 685.185 KHz */
  // /* then divide by 64 for 10.706 KHz */
  // /* configured using either PLL in either integer or fractional mode */

  // Serial.println("Set Output #2 to 10.706 KHz");  
  // clockgen.setupMultisynth(2, SI5351_PLL_B, 900, 0, 1);
  // clockgen.setupRdiv(2, SI5351_R_DIV_64);
    
  // /* Enable the clocks */
  // clockgen.enableOutputs(true);
  showPrompt("SG-1");
  boolean i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    Serial.println("Device not found on I2C bus!");
  }

  // Set CLK0 to output 14 MHz
  si5351.set_freq(1400000000ULL, SI5351_CLK0);
  si5351.update_status();
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