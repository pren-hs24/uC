#include "driveStraight.h"

void driveStraight(float distance_mm) {
  long encoderLeftStart = motorLeft.getEncoderPosition();
  long encoderRightStart = motorRight.getEncoderPosition();

  float ticksPerMM = TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE;
  float targetTicks = distance_mm * ticksPerMM;

  while (true) {
    long deltaL = motorLeft.getEncoderPosition() - encoderLeftStart;
    long deltaR = motorRight.getEncoderPosition() - encoderRightStart;

    long avgTicks = (deltaL + deltaR) / 2;
    float error_forward = targetTicks - avgTicks;
    float error_sync = deltaL - deltaR;

    float baseRPM = MIN_RPM + error_forward;
    float correctionRPM = KP_SYNC * error_sync;

    float rpmL = constrain(baseRPM - correctionRPM, -MAX_RPM, MAX_RPM);
    float rpmR = constrain(baseRPM + correctionRPM, -MAX_RPM, MAX_RPM);

    motorLeft.setTargetRPM(rpmL);
    motorRight.setTargetRPM(rpmR);

    if (abs(error_forward) < 50) {  // tolerance in ticks
      motorLeft.setTargetRPM(0);
      motorRight.setTargetRPM(0);
      return;
    }

    delay(50);
  }
}

