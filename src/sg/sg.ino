
static int mode = 0;

class Button {
public:
  Button(int pin, boolean pullup=true):btnPin(pin),btnState(pullup?HIGH:LOW),pressedValue(pullup?LOW:HIGH) {
    pinMode(pin, pullup?INPUT_PULLUP:INPUT);
  }
  boolean isPressed() {
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

    if ( reading == pressedValue && (millis() - lastDebounceTime) > debounceDelay ) {
      //  been pressed longer enough
      isValid = true;
    }

    return ret;
  }
private:
  int btnPin;
  int btnState;
  int pressedValue;
  unsigned long lastDebounceTime = 0;
  boolean isValid = false;
  const unsigned long debounceDelay = 50;
};

Button btnMode(5);
Button btnStep(4);

void setup() {
//  pinMode(btnMode, INPUT_PULLUP);
  Serial.begin(9600);
}

int step = 1;

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
}


/*
boolean isKeyPressed()
{
  static int btnState = HIGH;
  static unsigned long lastDebounceTime = 0;  // the last time the button pin was toggled
  static boolean isValid = false;
  static const unsigned long debounceDelay = 50;
  int reading = digitalRead(btnMode);
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

  if ( reading == LOW && (millis() - lastDebounceTime) > debounceDelay ) {
    //  been pressed longer enough
    isValid = true;
  }

  return ret;
}
*/
