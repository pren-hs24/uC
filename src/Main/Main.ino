#include "FahrBefehle.h"
#include <Arduino.h>
#include <Wire.h>
#include "TOF.h"
#include "Servo.h"
#include "LineSensor.h"
#include "config.h"
#include "Endschalter.h"

ServoMotor myServo(2);

//I2C BUS
#define Wire_Bus Wire2
// #define I2C_addr_1 0x30
// #define I2C_addr_2 0x31
#define XSHUT_2 38
#define XSHUT_3 39

Endschalter endschalter(ENDSCHALTER_PIN);

Motor motorLeft(3, 4, 10, 11);
Motor motorRight(5, 6, 12, 13);

const uint16_t addr_1 = 0x30;
const uint16_t addr_2 = 0x31;

//ToF Sensoren instanzieren
TOF tof_front_upper(INT_TOF_1, XSHUT_TOF_1);
TOF tof_front_lower(INT_TOF_2, XSHUT_TOF_2);
TOF tof_back(INT_TOF_3, XSHUT_TOF_3);

LineSensor frontSensor;
LineSensor backSensor;



// Timer
IntervalTimer pwmTimerLeft;
IntervalTimer pwmTimerRight;
IntervalTimer controlTimer;
IntervalTimer rpmCallback;

// void callbackRPM(){
//   motorLeft.updateRPM();
//   motorRight.updateRPM();
// }

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

  //Endschalter
  endschalter.begin();

  //Servomotor
  myServo.begin();

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starte serielle kommunikation");

  //Liniensensor
  frontSensor.begin(frontPins, 5);
  backSensor.begin(backPins, 5);


  //Servomotor
  myServo.begin();

  // Timer zuweisen
  motorLeft.attachTimer(&pwmTimerLeft);
  motorRight.attachTimer(&pwmTimerRight);

  // Regel-Timer starten (10ms = 100 Hz)
  //controlTimer.begin(callbackRPM,1000);
  controlTimer.begin(controlCallback, 10000);


  //I2C
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starte serielle kommunikation");

  Wire_Bus.begin();
  Wire_Bus.setClock(100000);

  Serial.println("VL53L3CX Initialisierung...");

  tof_front_upper.setAddress();
  tof_front_lower.setAddress();
  tof_back.setAddress();

  tof_front_upper.init();
  tof_front_lower.init();
  tof_back.init();
 
  
  // for (int speed = 10; speed <= 50; speed+=10) {
  //   followLine((float)speed, tofFront, tofBack);
  //   delay(20);
  // }
}


void loop() {
 followLine(30, tof_front_lower, tof_back);


}