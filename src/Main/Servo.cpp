#include "Servo.h"
#include <Arduino.h>

ServoMotor::ServoMotor(int pin) {
  _pin = pin;
  _currentAngle = 5;
}

void ServoMotor::begin() {
  _servo.attach(_pin);       // korrekter PWM-Ausgang
  _servo.write(_currentAngle);
}

void ServoMotor::moveTo(int angle) {
  angle = constrain(angle, 0, 180);
  int step = (angle > _currentAngle) ? 1 : -1;

  for (int pos = _currentAngle; pos != angle; pos += step) {
    _servo.write(pos);     // erzeugt sauberes 50 Hz Servo-Signal
    delay(20);             // weiche Bewegung (20ms = 50Hz)
  }

  _currentAngle = angle;
}

void ServoMotor::liftHindernis() {
  moveTo(_currentAngle + 18);
  delay(2000);
}

void ServoMotor::dropHindernis() {
  moveTo(_currentAngle - 18);
  delay(2000);
}
