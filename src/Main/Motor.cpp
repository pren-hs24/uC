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
  : in1(in1), in2(in2), encoder(encA, encB), currentRPM(0), targetRPM(0), integral(0), lastError(0),
    lastUpdateTime(0), lastPosition(0), pwmState(false), timerActive(false), pwmDuty(0), direction(0), pwmTimer(nullptr) {
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
  float pwmFreq = 1000.0;
  float period_us = 1e6 / pwmFreq;

  if (!timerActive) return;

  if (pwmState) {
    // Driving
    if (direction > 0) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    float onTime = period_us * (pwmDuty / 255.0);
    if (timerActive) {
      pwmTimer->begin((this == instance0) ? pwmISRWrapper0 : pwmISRWrapper1, onTime);
    }
  } else {
    // Braking
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    float offTime = period_us * (1.0 - pwmDuty / 255.0);
    if (timerActive) {
      pwmTimer->begin((this == instance0) ? pwmISRWrapper0 : pwmISRWrapper1, offTime);
    }
  }

  pwmState = !pwmState;
}

void Motor::updateRPM() {
  unsigned long currentTime = micros();
  float dt = (currentTime - lastUpdateTime) / 1e6;

  // if (dt > 0.01) {
  int currentPosition = encoder.read();
  int deltaTicks = currentPosition - lastPosition;
  int ticksPerSecond = deltaTicks / dt;
  currentRPM = (ticksPerSecond / (float)ticksPerRevolution) * 60.0;
  lastPosition = currentPosition;
  lastUpdateTime = currentTime;
  // }
}

void Motor::setTargetRPM(float rpm) {
  targetRPM = rpm;

  // === Sofortiger Stopp bei Ziel 0 ===
  if (abs(rpm) < 1.0) {
    if (pwmTimer) pwmTimer->end();
    timerActive = false;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    integral = 0;
    lastError = 0;
    direction = 0;
    pwmDuty = 0;
    return;
  }

  // === Start aus Stillstand ===
  if (!timerActive) {
    direction = (rpm > 0) ? 1 : -1;

    // RPM aktualisieren, aber wird ggf. ignoriert (siehe updateRPM())
    updateRPM();

    // Regler sauber zurücksetzen
    integral = 0;
    lastError = 0;

    // Initial PWM aus updateControl()
    updateControl();

    float pwmFreq = 10000.0;
    float period_us = 1e6 / pwmFreq;
    float onTime = period_us * (pwmDuty / 255.0);

    pwmState = true;
    pwmTimer->begin((this == instance0) ? pwmISRWrapper0 : pwmISRWrapper1, onTime);
    timerActive = true;
  }
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
  //static float integral = 0;
  //static float lastError = 0;

  float error = targetRPM - currentRPM;

  float Kp = 0.0267;
  float Ki = 0.1935;
  float Kd = 0.0;

  // Anti-Windup: Integrator nur wenn RPM stabil
  //bool integratorAllowed = abs(error) < 100.0 && (abs(currentRPM) > 2.0 || abs(error) > 20.0);

  //if (integratorAllowed) {
    integral += error;
    integral = constrain(integral, -3000, 3000);
  //}
  //float derivative = (error - lastError) / 0.01;
  lastError = error;

  //   // Optional: Reset Integrator bei Überschwingen
  // if ((currentRPM > targetRPM + 10) && (error < 0)) {
  //   integral = 0;
  // }

  float Ki_dynamic = Ki;//(integratorAllowed) ? 0.0 : Ki;

  float controlSignal = Kp * error + Ki_dynamic * integral;
  float absSignal = abs(controlSignal);

  int newDir = 0;
  if (targetRPM > 1.0) newDir = 1;
  else if (targetRPM < -1.0) newDir = -1;

  if (abs(targetRPM) < 1.0) {
    if (pwmTimer) pwmTimer->end();
    pwmDuty = 0;
    timerActive = false;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    integral = 0;
    direction = 0;
    return;
  }

  direction = newDir;
  pwmDuty = constrain(absSignal, 0, 245);

  Serial.printf("Current RPM: %.2f | targetRPM: %.2f | pwmDuty: %.2f | integral: %.2f\n",
                currentRPM,targetRPM,pwmDuty, integral);

  // if(this==instance0){
  //   Serial.printf("rpm_L = %.2f\n",currentRPM);
  // }
  // else{
  //   Serial.printf("rpm_R = %.2f\n",currentRPM);
  // }
}
