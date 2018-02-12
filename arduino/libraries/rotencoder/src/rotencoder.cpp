/*
  rotencoder.h - Library for rotate encoder.
  Created by Weng Kai, Feb. 8, 2018.
  Released into the public domain.
*/

#include "rotencoder.h"

static RotateEncoder* pRE0 = 0;
static RotateEncoder* pRE1 = 0;

RotateEncoder::RotateEncoder(int pin_clk, int pin_dt):
  pinClk(pin_clk), pinDT(pin_dt) {
  pinMode(pin_clk, INPUT_PULLUP);
  pinMode(pin_dt, INPUT_PULLUP);
  if ( pin_clk == 2 ) {
    attachInterrupt(0, isp_clk0, CHANGE); 
    pRE0 = this;
  } else if ( pin_clk == 3 ) {
    attachInterrupt(1, isp_clk1, CHANGE); 
    pRE1 = this;
  }
}

void RotateEncoder::clk_pulse() {
  int clk = digitalRead(pinClk);
  int dt = digitalRead(pinDT);
  if ( clk != lastClk ) {
    lastClk = clk;
    count += (clk != dt)? step:-step;
  }
}

//  interrupt routings
void RotateEncoder::isp_clk0() {
  if ( pRE0 ) {
    pRE0->clk_pulse();
  }
}

void RotateEncoder::isp_clk1() {
  if ( pRE1 ) {
    pRE1->clk_pulse();
  }
}