#pragma once

#include "Motor.h"
#include "LineSensor.h"


// Externe Objekt-Deklarationen (einmal definiert, woanders benutzt)
extern Motor motorLeft;
extern Motor motorRight;
extern LineSensor lineSensor;

// Funktions-Deklarationen
void followLine(float baseSpeed);
void followLineBackwards(float baseSpeed);
void rotateUntilLine();
void rotateUntilLine360();
