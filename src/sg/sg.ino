#include <button.h>
#include <rotencoder.h>

//  Button---------------------------------

Button btnMode(5);
Button btnStep(4);

//  RotateEncoder--------------------------

RotateEncoder re(2,3);

void setup() {
  Serial.begin(9600);
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