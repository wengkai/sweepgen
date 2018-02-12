/*
  rotencoder.h - Library for rotate encoder.
  Created by Weng Kai, Feb. 8, 2018.
  Released into the public domain.
*/

#ifndef _ROTENCODER_H_
#define _ROTENCODER_H_

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

class RotateEncoder {
public:
  RotateEncoder(int pin_clk, int pin_dt);
  void setCount(long count) { this->count = count; }
  long getCount() { return count; }
  void setStep(long step) { this->step = step; }
private:
  static void isp_clk0();
  static void isp_clk1();
  void clk_pulse();
  int pinClk;
  int pinDT;
  unsigned long lastT = 0;
  int lastClk = 0;  
  long count = 0;
  long step = 1;
};

#endif