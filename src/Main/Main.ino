#include "FahrBefehle.h"
#include <Arduino.h>
#include "Servo.h"
#include <Wire.h>
#include "TOF.h"


unsigned long lastUpdate = 0;

ServoMotor myServo(2);


 bool delayNonBlocking(unsigned long dauer) {
  static unsigned long startzeit = 0;
  static bool läuft = false;

  if (!läuft) {
    startzeit = millis();
    läuft = true;
    return false;
  }

  if (millis() - startzeit >= dauer) {
    läuft = false; // zurücksetzen für nächsten Aufruf
    return true;
  }

  return false;
}

// Timer
IntervalTimer pwmTimerLeft;
IntervalTimer pwmTimerRight;
IntervalTimer controlTimer;

// Regel-Callback (100 Hz)
void controlCallback() {
  motorLeft.updateRPM();
  motorLeft.updateControl();

  motorRight.updateRPM();
  motorRight.updateControl();
/*
  // Serielle Ausgabe (optional)
  Serial.print("L: ");
  Serial.print(motorLeft.getRPM());
  Serial.print(" RPM | R: ");
  Serial.print(motorRight.getRPM());
  Serial.println(" RPM");
*/
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starte serielle kommunikation");

  // Liniensensor
  lineSensor.begin();

  //Servomotor
  myServo.begin();

  // Timer zuweisen
  motorLeft.attachTimer(&pwmTimerLeft);
  motorRight.attachTimer(&pwmTimerRight);

  // Regel-Timer starten (10ms = 100 Hz)
 controlTimer.begin(controlCallback, 10000);




}


void loop() {
  
 followLine(40);
 


 


  
}







/*
   float baseSpeed =40.00;  // Grundgeschwindigkeit in RPM
  const float Kp = 0.02;     // Verstärkung für Lenkung
  const int center = 5000;   // Mitte der Linie


  uint16_t position = lineSensor.readPosition();
  //Serial.println(position);

  int error = position - center;
  float correction = Kp * error;
  Serial.println(correction);
  correction = constrain(correction, -30, 30);  // sanft halten
  
  float leftSpeed = baseSpeed + correction;
  float rightSpeed = baseSpeed - correction;
  //Serial.println(leftSpeed);
  //Serial.println(rightSpeed);

  motorLeft.setTargetRPM(-leftSpeed);
  motorRight.setTargetRPM(rightSpeed);
 
 
  // Regelung alle 10 ms
  if (millis() - lastUpdate >= 10) {
    motorLeft.updateRPM();
    motorLeft.updateControl();
    motorRight.updateRPM();
    motorRight.updateControl();
    lastUpdate = millis();
  }
*/

