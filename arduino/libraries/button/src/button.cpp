/*
  button.cpp - Library for button debounce.
  Created by Weng Kai, Feb. 8, 2018.
  Released into the public domain.
*/

#include "button.h"

static const unsigned long debounceDelay = 50;

boolean Button::isPressed() {
  int reading = digitalRead(btnPin);
  int ret = false;

  // If the switch changed, due to noise or pressing:
  if ( reading != btnState ) {
    lastDebounceTime = millis();
    if ( isValid ) {
      //  was valid and released now
      ret = true;
    }
    isValid = false;
    btnState = reading;
  }

  if ( reading == pressedValue && 
  	(millis() - lastDebounceTime) > debounceDelay ) {
    //  been pressed longer enough
    isValid = true;
  }

  return ret;
}