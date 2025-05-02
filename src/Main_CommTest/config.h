// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Pin-Zuweisungen-------------------------------

//H-Brücke links
#define IN1_1 3
#define IN2_1 4

//H-Brücke rechts
#define IN1_2 5
#define IN2_2 6

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

enum Target : uint8_t {
    TARGET_A = 0,
    TARGET_B = 1,
    TARGET_C = 2
};


#endif
