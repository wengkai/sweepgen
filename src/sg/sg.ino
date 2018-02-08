#include <rotencoder.h>

#include <button.h>

//
////  Button---------------------------------
//
//class Button {
//public:
//  Button(int pin, boolean pullup=true):btnPin(pin),btnState(pullup?HIGH:LOW),pressedValue(pullup?LOW:HIGH) {
//    pinMode(pin, pullup?INPUT_PULLUP:INPUT);
//  }
//  boolean isPressed() {
//    int reading = digitalRead(btnPin);
//    int ret = false;
//  
//    // If the switch changed, due to noise or pressing:
//    if ( reading != btnState ) {
//      lastDebounceTime = millis();
//      if ( isValid ) {
//        //  was valid and released now
//        ret = true;
//      }
//      isValid = false;
//      btnState = reading;
//    }
//
//    if ( reading == pressedValue && (millis() - lastDebounceTime) > debounceDelay ) {
//      //  been pressed longer enough
//      isValid = true;
//    }
//
//    return ret;
//  }
//private:
//  int btnPin;
//  int btnState;
//  int pressedValue;
//  unsigned long lastDebounceTime = 0;
//  boolean isValid = false;
//  const unsigned long debounceDelay = 50;
//};

//  Button---------------------------------

Button btnMode(5);
Button btnStep(4);

//  RotateEncoder--------------------------

//int ok= 0;

//class RotateEncoder {
//public:
//  RotateEncoder(int pin_clk, int pin_dt):
//    pinClk(pin_clk), pinDT(pin_dt) {
//    pinMode(pin_clk, INPUT_PULLUP);
//    pinMode(pin_dt, INPUT_PULLUP);
//    if ( pin_clk == 2 ) {
//      attachInterrupt(0, isp_clk0, CHANGE); 
//      pRE0 = this;
//    } else {
//      attachInterrupt(1, isp_clk1, CHANGE); 
//      pRE1 = this;
//    }
//  }
//  int getCount() { return count; }
//  static void isp_clk0() {
//    if ( pRE0 ) {
//      pRE0->clk_pulse();
//    }
//  }
//  static void isp_clk1() {
//    if ( pRE1 ) {
//      pRE1->clk_pulse();
//    }
//  }
//private:
//  void clk_pulse() {
//    int clk = digitalRead(pinClk);
//    int dt = digitalRead(pinDT);
//    if ( clk != lastClk ) {
//      lastClk = clk;
//      count += (clk != dt)? 1:-1;
//   }
//  }
//  int pinClk;
//  int pinDT;
//  unsigned long lastT = 0;
//  int lastClk = 0;  
//  int count = 0;
//  static RotateEncoder* pRE0;
//  static RotateEncoder* pRE1;
//};
//
//RotateEncoder* RotateEncoder::pRE0 = 0;
//RotateEncoder* RotateEncoder::pRE1 = 0;

//  RotateEncoder--------------------------

RotateEncoder re(2,3);

void setup() {
  Serial.begin(9600);
//  pinMode(2, INPUT_PULLUP);
//  pinMode(3, INPUT_PULLUP);
//  attachInterrupt(0, isp, CHANGE);
//  ok=1;
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
  }
  
}

//int lastclk=0;
//
//void isp()
//{
//  int clk = digitalRead(2);
//  int dt = digitalRead(3);
//  
//  if ( lastclk != clk ) {
////    Serial.print(clk);
////    Serial.print(":");
////    Serial.println(dt);
//    if ( clk == dt ) freq--;
//    else freq++;
//    lastclk = clk;
//  }
//}
