#include "FahrBefehle.h"

// Objekt-Definitionen (nur HIER!)
// Motor motorLeft(3, 4, 10, 11);
// Motor motorRight(5, 6, 12, 13);

int lastError = 0;        // bleibt zwischen den Aufrufen erhalten
float integral = 0;

bool followLine(float baseSpeed) {

  uint16_t distance_front_lower = UINT16_MAX;
  uint16_t distance_front_upper = UINT16_MAX;
  const float Kp = 0.03;  // Noch etwas kleiner
  const float Kd = 0.0;    // Dämpfung erhöhen
  const float Ki = 0.0;    // Ki erstmal weglassen, nur wenn systematisch ein Versatz bleibt
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

  motorLeft.setTargetRPM(-leftSpeed);
  motorRight.setTargetRPM(rightSpeed);
  
  if(frontSensor.onPoint()){//offset 6cm
  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500); 
  driveStraight1(70);
  delay(500);
  return true;
  }

  if(frontSensor.isOnLine1()){
   rotateUntilLine360(); 
  }

  if (!tof_back.meassure(&distance_front_lower)) {
    Serial.println("Fehler in Messung TOF Front");
  }

  if(!tof_front_upper.meassure(&distance_front_upper)){
    Serial.println("Fehler oberer TOF front");
  }

  Serial.println(distance_front_lower);

  if(distance_front_lower < DET_DIST){
    motorLeft.setTargetRPM(0);
    motorRight.setTargetRPM(0);
    UART_SendEvent(EVT_NEXT_POINT_BLOCKED,nullptr,0);
    rotateUntilLine();
    UART_SendEvent(EVT_RETURNING,nullptr,0);
    return false;
  }

  if(distance_front_lower < 300){
    UART_SendEVENT(EVT_OBSTACLE_DETECTED,nullptr,0);
    motorLeft.setTargetRPM(0);
    motorRight.setTargetRPM(0);
    delay(2000);
    ablaufHindernis();
  }

  return false;
}

void followLineBackwards(float baseSpeed) {
  const float Kp = 0.02;
  const float Kd = 0.0;
  const float Ki = 0.0;
  const int center = 2000;
  uint16_t distance_back = 0;

  
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

  if (!tof_front_lower.meassure(&distance_back)) {
    Serial.println("Fehler in Messung TOF Front");
  }

  Serial.println(distance_back);

  if(endschalter.isPressed()){
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

void rotateUntilLineDirect() {

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
  Serial.println("Starte Drehung zur Liniensuche…");

  motorLeft.setTargetRPM(-20);
  motorRight.setTargetRPM(-20);
  

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



void rotateUntilLineBack() {

  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(500);
  Serial.println("Starte Drehung zur Liniensuche…");

  motorLeft.setTargetRPM(-20);
  motorRight.setTargetRPM(-20);
  delay(500);

  while (true) {
    uint16_t position = backSensor.readPosition();
    //bool onLine = frontSensor.isOnLine();  // dein robuster Check
    Serial.println(position);
    Serial.println(backSensor.isOnLine());

    if (backSensor.isOnLine() && position >= 1700 && position <= 2300) {
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



void ablaufHindernis(){

  rotateUntilLineBack();
  followLineBackwards(15);
  delay(500);
  myServo.liftHindernis();
  delay(500);
  rotateUntilLineBack();//offset von 20 cm
  delay(500);
  driveStraight(210);
  delay(500);
  myServo.dropHindernis();
  delay(500);
  followLine(40);


}


