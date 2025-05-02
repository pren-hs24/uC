#include "Motor.h"

const int ticksPerRevolution = 2184;

// Globale Wrapper-Instanzen
Motor* Motor::instance0 = nullptr;
Motor* Motor::instance1 = nullptr;

// === Wrapper-ISRs ===
void Motor::pwmISRWrapper0() {
  if (instance0) instance0->pwmISR();
}

void Motor::pwmISRWrapper1() {
  if (instance1) instance1->pwmISR();
}

// === Konstruktor ===
Motor::Motor(int in1, int in2, int encA, int encB)
  : in1(in1), in2(in2), encoder(encA, encB), currentRPM(0), targetRPM(0),
    lastUpdateTime(0), lastPosition(0), pwmDuty(0), direction(0), pwmTimer(nullptr) {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void Motor::attachTimer(IntervalTimer* t) {
  pwmTimer = t;
  if (!instance0) {
    instance0 = this;
    pwmTimer->begin(pwmISRWrapper0, 1000);
    pwmTimer->end();
  } else if (!instance1) {
    instance1 = this;
    pwmTimer->begin(pwmISRWrapper1, 1000);
    pwmTimer->end();
  }
}

void Motor::pwmISR() {
  static bool pwmState = false;

  if (pwmState) {
    // Driving
    if (direction > 0) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  } else {
    // Braking
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }

  pwmState = !pwmState;
}

void Motor::updateRPM() {
  unsigned long currentTime = micros();
  float dt = (currentTime - lastUpdateTime) / 1e6;

  if (dt > 0.01) {
    int currentPosition = encoder.read();
    int deltaTicks = currentPosition - lastPosition;
    int ticksPerSecond = deltaTicks / dt;
    currentRPM = (ticksPerSecond / (float)ticksPerRevolution) * 60.0;

    lastPosition = currentPosition;
    lastUpdateTime = currentTime;
  }
}

void Motor::setTargetRPM(float rpm) {
  targetRPM = rpm;
}

float Motor::getRPM() {
  return currentRPM;
}

int Motor::getEncoderPosition() {
  return encoder.read();
}

int Motor::getDirection() {
  return direction;
}

void Motor::updateControl() {
  static float integral = 0;
  static float lastError = 0;

  float error = targetRPM - currentRPM;
  float Kp = 0.0825;
  float Ki = 0.0853;
  float Kd = 0.0;

  integral += error;
  integral = constrain(integral, -3000, 3000);
  float derivative = (error - lastError) / 0.01;
  lastError = error;

  float controlSignal = Kp * error + Ki * integral + Kd * derivative;
  float absSignal = abs(controlSignal);

  int newDir = 0;
  if (targetRPM > 1.0) newDir = 1;
  else if (targetRPM < -1.0) newDir = -1;

  if (abs(targetRPM) < 1.0) {
    if (pwmTimer) pwmTimer->end();
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    integral = 0;
    direction = 0;
    return;
  }

  direction = newDir;
  pwmDuty = constrain(absSignal, 10, 245);

  float pwmFreq = 10000.0;
  float period_us = 1e6 / pwmFreq;
  float onTime = period_us * (pwmDuty / 255.0);

  if (pwmTimer) {
    if (this == instance0) pwmTimer->begin(pwmISRWrapper0, onTime);
    else if (this == instance1) pwmTimer->begin(pwmISRWrapper1, onTime);
  }
}
