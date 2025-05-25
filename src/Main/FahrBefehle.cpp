#include "FahrBefehle.h"

// Objekt-Definitionen (nur HIER!)
// Motor motorLeft(3, 4, 10, 11);
// Motor motorRight(5, 6, 12, 13);

int lastError = 0;        // bleibt zwischen den Aufrufen erhalten
float integral = 0;

void followLine(float baseSpeed, TOF& tofFront, TOF& tofBack) {

  uint16_t distance_Back = 0;
  const float Kp = 0.005;   // kleiner machen für sanfteres Reagieren
  const float Kd = 0.0;    // Dämpft schnelle Änderungen
  const float Ki = 0.0;    // erstmal auf 0 lassen
  const int center = 2000;
  uint16_t position = frontSensor.readPosition();
  Serial.println(String("Positiooooooon: ") + position);
  int error = position - center;

  integral += error;                         // summiert Fehler über Zeit
  int derivative = error - lastError;        // wie stark sich der Fehler verändert
  lastError = error;

  float correction = Kp * error + Kd * derivative + Ki * integral;
  correction = constrain(correction, -30, 30);  // maximale Korrektur begrenzen

  float leftSpeed = baseSpeed + correction;
  float rightSpeed = baseSpeed - correction;

    if(frontSensor.onPoint()){
    motorLeft.setTargetRPM(0);
    motorRight.setTargetRPM(0);
    delay(1000); 
    return;
  }

  motorLeft.setTargetRPM(-leftSpeed);
  motorRight.setTargetRPM(rightSpeed);
  
  if(frontSensor.isOnLine1()){
   rotateUntilLine360(); 
  }

  if (!tofBack.meassure(&distance_Back)) {
    Serial.println("Fehler in Messung TOF Front");
  }

  Serial.println(distance_Back);

  if(distance_Back < 300){
    motorLeft.setTargetRPM(0);
    motorRight.setTargetRPM(0);
    delay(2000);
    ablaufHindernis(tofFront, tofBack);
  }

  
}

void followLineBackwards(float baseSpeed, TOF& tofFront, TOF& tofBack) {
  const float Kp = 0.0025;
  const float Kd = 0.0;
  const float Ki = 0.0;
  const int center = 2000;
  uint16_t distance_Back = 0;

  
  while(true){

  uint16_t position = backSensor.readPosition();
  

  Serial.println(String("BackSensor Position (gespiegelt): ") + position);

  int error = position - center;
  integral += error;
  int derivative = error - lastError;
  lastError = error;

  float correction = Kp * error + Kd * derivative + Ki * integral;
  correction = constrain(correction, -30, 30);

  float leftSpeed = baseSpeed + correction;
  float rightSpeed = baseSpeed - correction;

  // Rückwärtsfahrt → Geschwindigkeiten negativ setzen
  motorLeft.setTargetRPM(leftSpeed);
  motorRight.setTargetRPM(-rightSpeed);

  if (!tofFront.meassure(&distance_Back)) {
    Serial.println("Fehler in Messung TOF Front");
  }

  Serial.println(distance_Back);

  if(distance_Back < 50){
    motorLeft.setTargetRPM(0);
    motorRight.setTargetRPM(0);
    delay(500);
    return;
  }


  }
  
}


void rotateUntilLine() {

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
  Serial.println("Starte Drehung zur Liniensuche…");

  motorLeft.setTargetRPM(-20);
  motorRight.setTargetRPM(-20);
  delay(2000);

  while (true) {
    uint16_t position = frontSensor.readPosition();
    //bool onLine = frontSensor.isOnLine();  // dein robuster Check
    Serial.println(position);
    Serial.println(frontSensor.isOnLine());

    if (frontSensor.isOnLine() && position >= 1700 && position <= 2300) {
      Serial.println("Linie wirklich zentriert erkannt!");
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

  motorLeft.setTargetRPM(-20);
  motorRight.setTargetRPM(-20);
  delay(5000);
  

  while (true) {
    uint16_t position = frontSensor.readPosition();
    //bool onLine = frontSensor.isOnLine();  // dein robuster Check

    if (frontSensor.isOnLine() &&  position >= 1700 && position <= 2300) {
      Serial.println("Linie wirklich zentriert erkannt!");
      break;
    }

    

    delay(10);
  }

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
}



void ablaufHindernis(TOF& tofFront, TOF& tofBack){

  rotateUntilLine();
  followLineBackwards(15, tofFront, tofBack);
  delay(500);
  myServo.liftHindernis();
  delay(500);
  rotateUntilLine();
  delay(500);
  myServo.dropHindernis();
  followLine(40, tofFront, tofBack);


}


