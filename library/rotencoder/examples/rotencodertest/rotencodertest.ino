#include "rotencoder.h"

//  a rotencoder, CLK at D2, DT at D3
RotateEncoder re(2,3);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  static int count = 0;

  if ( re.getCount() != count ) {
    count = re.getCount();
    Serial.print("count:");
    Serial.println(count);
  }
}