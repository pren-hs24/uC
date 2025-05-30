#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "Motor.h"
#include "LineSensor.h"
#include "Servo.h"
#include "TOF.h"
#include "driveStraight.h"
#include "Endschalter.h"


// Externe Objekt-Deklarationen (einmal definiert, woanders benutzt)
extern Motor motorLeft;
extern Motor motorRight;
extern TOF tof_front_upper;
extern TOF tof_front_lower;
extern TOF tof_back;
extern LineSensor frontSensor;
extern LineSensor backSensor;
extern ServoMotor myServo;
extern Endschalter endschalter;


// Funktions-Deklarationen
void followLine(float baseSpeed, TOF& tof_front_lower, TOF& tof_back);
void followLineBackwards(float baseSpeed, TOF& tof_front_lower, TOF& tof_back);
void rotateUntilLine();
void rotateUntilLine360();
void ablaufHindernis(TOF& tof_front_lower, TOF& tof_back);


