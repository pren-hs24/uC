#include "Turn.h"

void Turn_UFO(float targetAngle) {
  // === Encoderwerte ===
  long encoderLeftStart = 0;
  long encoderRightStart = 0;
  long encoderLeft = 0;
  long encoderRight = 0;

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
    float baseRPM = MIN_RPM + KP_ANGLE * error_angle;
    float correctionRPM = KP_SYNC * error_sync;

    float rpmL = constrain(baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
    float rpmR = constrain(-baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);

    motorLeft.setTargetRPM(rpmL);
    motorRight.setTargetRPM(-rpmR);
    // setTargetRPM_Left(rpmL);
    // setTargetRPM_Right(rpmR);

    if (abs(error_angle) < 100) {  // Toleranz in Ticks
      motorLeft.setTargetRPM(0);
      motorLeft.setTargetRPM(0);
      return;
    }

    delay(50);
  }
}
