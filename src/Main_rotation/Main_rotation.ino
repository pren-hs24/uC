#include <Wire.h>
#include "config.h"
#include "Motor.h"
#include "UART.h"

Motor motorLeft(IN1_L,IN2_L,PHASE_A_L,PHASE_B_L);
Motor motorRight(IN1_R,IN2_R,PHASE_A_R,PHASE_B_R);
IntervalTimer timerLeft;
IntervalTimer timerRight;
IntervalTimer timerMotors;

// === Konstante Parameter ===
const float TICKS_PER_ROTATION = 2148;
const float WHEEL_DIAMETER = 94.2;     // mm
const float TRACK_WIDTH = 275.0;       // mm (Radabstand)
const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;

const float KP_ANGLE = 0.0008;           // kleiner als für PWM!
const float KP_SYNC  = 0;//0.005;

const float MAX_RPM = 25.0;
const float MIN_RPM = 15.0;

// === Encoderwerte ===
long encoderLeftStart = 0;
long encoderRightStart = 0;
long encoderLeft = 0;
long encoderRight = 0;

// === Ziel ===
float targetAngle = 360.0;
bool turning = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial comm gestartet");

  encoderLeftStart = motorLeft.getEncoderPosition();
  encoderRightStart = motorRight.getEncoderPosition();

  motorLeft.attachTimer(&timerLeft);
  motorRight.attachTimer(&timerRight);

  timerMotors.begin(updateMotors,10000);
  Serial.println("Starte drehung");

  motorLeft.setTargetRPM(120);
}

void loop() {
  // while (!turning);

  // long deltaL = motorLeft.getEncoderPosition() - encoderLeftStart;
  // long deltaR = -(motorRight.getEncoderPosition() - encoderRightStart);
  // long tickDiff = deltaL - deltaR;

  // // Ziel-Tickdifferenz berechnen
  // float ticksPerMM = TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE;
  // float targetDistance = (targetAngle * PI / 180.0) * (TRACK_WIDTH / 2.0);
  // float targetTicks = targetDistance * ticksPerMM * 2.0;

  // // Regelfehler
  // float error_angle = targetTicks - tickDiff;
  // float error_sync = (deltaL + deltaR);  // soll 0 sein

  // // Regelausgang als RPM
  // float baseRPM = MIN_RPM+KP_ANGLE * error_angle;
  // float correctionRPM = KP_SYNC * error_sync;

  // float rpmL = constrain(baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
  // float rpmR = constrain(-baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);

  // float rpmL_m = motorLeft.getRPM();
  // float rpmR_m = motorRight.getRPM();
  // Serial.printf("deltaL: %ld | deltaR: %ld |error_angle: %.1f | baseRPM: %.2f | rpmL: %.2f | rpmR: %.2f | rpmL_m: %.2f | rpmR_m: %.2f\n",
  //             deltaL, deltaR,error_angle, baseRPM, rpmL, rpmR,rpmL_m,rpmR_m);

  // // Serial.printf("Rpm Links: %f\n",rpmL);
  // // Serial.printf("Rpm rechts: %f\n",rpmR);
  // motorLeft.setTargetRPM(rpmL);
  // motorRight.setTargetRPM(-rpmR);
  // // setTargetRPM_Left(rpmL);
  // // setTargetRPM_Right(rpmR);

  // if (abs(error_angle) < 100) {  // Toleranz in Ticks
  //   motorLeft.setTargetRPM(0);
  //   motorRight.setTargetRPM(0);
  //   turning = false;
  //   Serial.println("Drehung abgeschlossen.");
  //   while(true);
  // }

  // delay(50);
}

void updateMotors(){
  motorLeft.updateRPM();
  motorRight.updateRPM();
  motorLeft.updateControl();
  motorRight.updateControl();
}
