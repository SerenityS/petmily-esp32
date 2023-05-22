// SR602 Motion Sensor

#define SR602_PIN 34

int pinStateCurrent = LOW;   // current state of pin
int pinStatePrevious = LOW;  // previous state of pin
bool motion_status = false;

void SR602_setup() {
  Serial.println("Initialize SR602...");

  pinMode(SR602_PIN, INPUT);
}

bool SR602_check() {
  pinStatePrevious = pinStateCurrent;        // store old state
  pinStateCurrent = digitalRead(SR602_PIN);  // read new state

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {  // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    motion_status = true;
  } else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {  // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    motion_status = false;
  }

  return motion_status;
}
