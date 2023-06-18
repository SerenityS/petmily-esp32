#include <ESP32_Servo.h>

static const int servoPin = 32;

Servo servo;
int deg, duty;

void Servo_init() {
  Serial.println("Initialize Servo Motor...");

  ledcSetup(0, 50, 16);
  ledcAttachPin(servoPin, 0);
}

void Servo_open() {
  for (deg = 90; deg >= 0; deg--) {
    servoWrite(0, deg);
  }
}

void Servo_close() {
  for (deg = 0; deg <= 90; deg++) {
    servoWrite(0, deg);
  }
}

void servoWrite(int ch, int deg) {
  duty = map(deg, 0, 180, 1638, 8192);
  ledcWrite(ch, duty);
  delay(30);  // delay를 줄이면 180도가 완전히 돌지 않음
}