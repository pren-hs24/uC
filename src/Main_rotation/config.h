// config.h
#ifndef CONFIG_H
#define CONFIG_H

// Pin-Zuweisungen-------------------------------

//Motor links
#define IN1_L 3
#define IN2_L 4
#define PHASE_A_L 10
#define PHASE_B_L 11

//Motor rechts
#define IN1_R 5
#define IN2_R 6
#define PHASE_A_R 12
#define PHASE_B_R 13

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


#endif
