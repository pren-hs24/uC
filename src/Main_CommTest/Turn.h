#include <Wire.h>
#include "config.h"
#include "Motor.h"
#include "FahrBefehle.h"

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


void Turn_UFO(float targetAngle);