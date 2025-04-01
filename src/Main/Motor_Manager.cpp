#include "Motor_Manager.h"

IntervalTimer MotorManager::timer;
std::vector<Motor*> MotorManager::motorList;

void MotorManager::begin(unsigned int interval_us) {
  timer.begin(updateAll, interval_us);
}

void MotorManager::registerMotor(Motor* motor) {
  motorList.push_back(motor);
}

void MotorManager::stop() {
  timer.end();
}

void MotorManager::updateAll() {
  for (Motor* motor : motorList) {
    if (motor) {
      motor->updateRPM();
      motor->updateControl();
    }
  }
}
