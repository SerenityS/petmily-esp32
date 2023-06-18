#include <Stepper.h>

const int stepsPerRevolution = 512;

Stepper myStepper(stepsPerRevolution, 27, 25, 26, 33);

void Step_init() {
  Serial.println("Initialize Step Motor...");
  myStepper.setSpeed(14);
}

void Step_open() {
  myStepper.step(stepsPerRevolution);
}

void Step_close() {
  myStepper.step(-stepsPerRevolution);
}