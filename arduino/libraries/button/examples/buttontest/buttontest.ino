#include "button.h"

//	a button at pin D4, pullup style
Button btn(4);

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if ( btn.isPressed() ) {
		Serial.println("button pressed.");
	}
}