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
    pwmTimer->begin(pwmISRWrapper0, 100);
    //pwmTimer->end();
  } else if (!instance1) {
    instance1 = this;
    pwmTimer->begin(pwmISRWrapper1, 100);
    //pwmTimer->end();
  }
}

void Motor::pwmISR() {
  pwmCounter++;
  if (pwmCounter < pwmOnTime) {
    // Motor EIN (je nach Richtung)
    if (direction > 0) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else if (direction < 0) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  } else {
    // Motor AUS (Bremsen)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }

  if (pwmCounter >= 255) pwmCounter = 0; // 8-Bit PWM-Zyklus
}

//Misst vie viele Encoder-Ticks in der Zeit dt vergangen sind, berechnet dan RPM
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

//Setzt Drehzahl des Motors
void Motor::setTargetRPM(float rpm) {
  targetRPM = rpm;
  //Serial.print("Target RPM gesetzt: ");
  //Serial.println(targetRPM);
}

//Gibt die aktuelle gemessene Drehzahl zurück
float Motor::getRPM() {
  return currentRPM;
}

//Gibt den aktuellen Encoderwert zurück
int Motor::getEncoderPosition() {
  return encoder.read();
}

//Gibt die aktuelle Richtung zurück
int Motor::getDirection() {
  return direction;
}


void Motor::updateControl() {
  static float integral = 0;
  static float lastError = 0;

  float error = targetRPM - currentRPM;

  // Reglerfaktoren
  float Kp = 0.0825;  // du kannst später erhöhen
  float Ki = 0.0;
  float Kd = 0.0;

  // Reglerteile
  integral += error;
  integral = constrain(integral, -3000, 3000);
  float derivative = (error - lastError) / 0.01;
  lastError = error;

  float controlSignal = Kp * error + Ki * integral + Kd * derivative;

  // === FEEDFORWARD-ANTEIL: direkt aus targetRPM
  float maxRPM = 240.0;  // maximal erreichbare RPM deines Motors
  float basePWM = map(abs(targetRPM), 0, maxRPM, 10, 255);  // grobe Umrechnung

  // === Gesamt-PWM: Basis + Regelung
  float rawPWM = basePWM + controlSignal;

  // Richtung bestimmen
  int newDir = 0;
  if (targetRPM > 1.0) newDir = 1;
  else if (targetRPM < -1.0) newDir = -1;

  direction = newDir;

  // PWM begrenzen und anwenden
  pwmDuty = constrain(abs(rawPWM), 20, 255);
  pwmOnTime = pwmDuty;

  // Debug (optional)
  /*
  Serial.print("targetRPM: "); Serial.print(targetRPM);
  Serial.print(" | currentRPM: "); Serial.print(currentRPM);
  Serial.print(" | pwmDuty: "); Serial.println(pwmDuty);
  */
}
