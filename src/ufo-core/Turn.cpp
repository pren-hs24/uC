#include "Turn.h"

void Turn_UFO(float targetAngle) {
  // === Encoderwerte ===
  long encoderLeftStart = 0;
  long encoderRightStart = 0;
  long encoderLeft = 0;
  long encoderRight = 0;

  int direction = 0;  //initalisierung

  float baseRPM = 0;
  float rpmR=0;
  float rpmL=0;

  if (targetAngle > 0) {
    direction = 1;
  } else if (targetAngle < 0) {
    direction = -1;
  }
  else{
    //winkel = 0 -> Funktion verlassen
    return;
  }

  encoderLeftStart = motorLeft.getEncoderPosition();
  encoderRightStart = motorRight.getEncoderPosition();

  while (true) {

    long deltaL = motorLeft.getEncoderPosition() - encoderLeftStart;
    long deltaR = -(motorRight.getEncoderPosition() - encoderRightStart);
    long tickDiff = deltaL - deltaR;

    // Ziel-Tickdifferenz berechnen
    float ticksPerMM = TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE;
    float targetDistance = (targetAngle * PI / 180.0) * (TRACK_WIDTH / 2.0);
    float targetTicks = targetDistance * ticksPerMM * 2.0;

    // Regelfehler
    float error_angle = targetTicks - tickDiff;
    float error_sync = (deltaL + deltaR);  // soll 0 sein

    // Regelausgang als RPM
    float angleRPM = KP_ANGLE * error_angle;
    float correctionRPM = KP_SYNC * error_sync;
    if (direction == 1) {
      baseRPM = MIN_RPM + angleRPM;
      rpmL = constrain(baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
      rpmR = constrain(-baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
    } else if (direction == -1) {
      baseRPM = angleRPM - MIN_RPM;
      rpmL = constrain(baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
      rpmR = constrain(-baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
    }
    else{
      rpmL=0;
      rpmR=0;
    }
    motorLeft.setTargetRPM(rpmL);
    motorRight.setTargetRPM(-rpmR);
    // setTargetRPM_Left(rpmL);
    // setTargetRPM_Right(rpmR);

    if (abs(error_angle) < 100) {  // Toleranz in Ticks
      motorLeft.setTargetRPM(0);
      motorRight.setTargetRPM(0);
      return;
    }

    delay(50);
  }
}
