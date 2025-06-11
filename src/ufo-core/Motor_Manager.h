#pragma once

#include <Arduino.h>
#include "Motor.h"
#include <vector>

class MotorManager {
public:
  static void begin(unsigned int interval_us = 10000);  // Startet den Timer (Standard: 10ms)
  static void registerMotor(Motor* motor);              // Fügt einen Motor hinzu
  static void stop();                                   // Stoppt den Timer (optional)

private:
  static void updateAll();                              // Timer-Callback
  static IntervalTimer timer;
  static std::vector<Motor*> motorList;
};
