#include "FahrBefehle.h"
#include <Arduino.h>

// Objekt-Definitionen (nur HIER!)
Motor motorLeft(3, 4, 10, 11);
Motor motorRight(5, 6, 12, 13);
LineSensor lineSensor;

int lastError = 0;        // bleibt zwischen den Aufrufen erhalten
float integral = 0;

void followLine(float baseSpeed) {
  const float Kp = 0.01;   // kleiner machen für sanfteres Reagieren
  const float Kd = 0.0;    // Dämpft schnelle Änderungen
  const float Ki = 0.0;    // erstmal auf 0 lassen
  const int center = 5000;

  uint16_t position = lineSensor.readPosition();
  int error = position - center;

  integral += error;                         // summiert Fehler über Zeit
  int derivative = error - lastError;        // wie stark sich der Fehler verändert
  lastError = error;

  float correction = Kp * error + Kd * derivative + Ki * integral;
  correction = constrain(correction, -30, 30);  // maximale Korrektur begrenzen

  float leftSpeed = baseSpeed + correction;
  float rightSpeed = baseSpeed - correction;

  motorLeft.setTargetRPM(-leftSpeed);
  motorRight.setTargetRPM(rightSpeed);
  
  if(!lineSensor.isOnLine()){
    rotateUntilLine360(); 
  }

  if(!lineSensor.onPoint()){
    delay(10000); 
  }

  
}

void followLineBackwards(float baseSpeed) {
  const float Kp = 0.3;
  const int center = 5000;

  uint16_t position = lineSensor.readPosition();
  Serial.println(position);
  int error = -(position - center);

  float correction = Kp * error;
  correction = constrain(correction, -30, 30);

  float leftSpeed = baseSpeed - correction;
  float rightSpeed = baseSpeed + correction;

  motorLeft.setTargetRPM(leftSpeed);
  motorRight.setTargetRPM(-rightSpeed);

 
}

void rotateUntilLine() {

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(2000);
  Serial.println("Starte Drehung zur Liniensuche…");

  motorLeft.setTargetRPM(15);
  motorRight.setTargetRPM(20);
  unsigned long startTime = millis();

  while (true) {
    uint16_t position = lineSensor.readPosition();
    bool onLine = lineSensor.isOnLine();  // dein robuster Check

    if (onLine && position >= 4800 && position <= 5200) {
      Serial.println("Linie wirklich zentriert erkannt!");
      break;
    }

    if (millis() - startTime > 10000) {
      Serial.println("Timeout: keine Linie erkannt.");
      break;
    }

    delay(10);
  }

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
}


void rotateUntilLine360() {

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(2000);
  Serial.println("Starte Drehung zur Liniensuche…");

  motorLeft.setTargetRPM(20);
  motorRight.setTargetRPM(20);
  delay(7000);
  unsigned long startTime = millis();

  while (true) {
    uint16_t position = lineSensor.readPosition();
    bool onLine = lineSensor.isOnLine();  // dein robuster Check

    if (onLine && position >= 4800 && position <= 5200) {
      Serial.println("Linie wirklich zentriert erkannt!");
      break;
    }

    if (millis() - startTime > 10000) {
      Serial.println("Timeout: keine Linie erkannt.");
      break;
    }

    delay(10);
  }

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
}