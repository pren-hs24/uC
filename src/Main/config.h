// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Pin-Zuweisungen-------------------------------

//Motor links
#define IN1_L 3
#define IN2_L 4
#define PHASE_A_L 10
#define PHASE_B_L 11

//Motor rechts
#define IN1_R 5
#define IN2_R 6
#define PHASE_A_R 10
#define PHASE_B_R 11

//Servo
#define SERVO 2
// Buzzer
#define BUZZER 7
//Start Button
#define START 27
//Zielschalter
#define GOAL_A 28
#define GOAL_B 29
#define GOAL_C 30

//Endschalter
#define ENDSCHALTER_PIN 34

//TOF1
#define INT_TOF_1 31
#define XSHUT_TOF_1 35
//TOF2
#define INT_TOF_2 32
#define XSHUT_TOF_2 38
//TOF3
#define INT_TOF_3 33
#define XSHUT_TOF_3 39

// Konstanten------------------------------------
#define MAX_SPEED 240
#define DEFAULT_SPEED 80
#define TURN_DIST 50
#define PICK_DIST 20
#define DET_DIST 3000

enum Target : uint8_t {
    TARGET_A = 0,
    TARGET_B = 1,
    TARGET_C = 2
};

const uint16_t addr_f_upper = 0x30;
const uint16_t addr_f_lower = 0x31;
const uint16_t addr_b = 0x32;

const uint8_t frontPins[] = {15, 17, 19, 21, 23};
const uint8_t backPins[] = {14, 16, 18, 20, 22};

const float TICKS_PER_ROTATION = 2148;
const float WHEEL_DIAMETER = 94.2;     // mm
const float TRACK_WIDTH = 275.0;       // mm
const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER;

const float KP_ANGLE = 0.0008;
const float KP_SYNC  = 0.005;

const float MAX_RPM = 25.0;
const float MIN_RPM = 15.0;


#endif
