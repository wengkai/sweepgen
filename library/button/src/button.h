/*
  button.h - Library for button debounce.
  Created by Weng Kai, Feb. 8, 2018.
  Released into the public domain.
*/

#ifndef _BUTTON_H_
#define _BUTTON_H_

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

class Button {
public:
  Button(int pin, boolean pullup=true):
  	btnPin(pin),btnState(pullup?HIGH:LOW),pressedValue(pullup?LOW:HIGH) {
    pinMode(pin, pullup?INPUT_PULLUP:INPUT);
  }
  boolean isPressed();
private:
  int btnPin;
  int btnState;
  int pressedValue;
  unsigned long lastDebounceTime = 0;
  boolean isValid = false;
};

#endif