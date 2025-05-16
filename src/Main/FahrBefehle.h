#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "Motor.h"
#include "LineSensor.h"
#include "Servo.h"
#include "TOF.h"


// Externe Objekt-Deklarationen (einmal definiert, woanders benutzt)
extern Motor motorLeft;
extern Motor motorRight;
extern LineSensor frontSensor;
extern LineSensor backSensor;
extern ServoMotor myServo;


// Funktions-Deklarationen
void followLine(float baseSpeed, TOF& tofFront, TOF& tofBack);
void followLineBackwards(float baseSpeed, TOF& tofFront, TOF& tofBack);
void rotateUntilLine();
void rotateUntilLine360();
void ablaufHindernis(TOF& tofFront, TOF& tofBack);


