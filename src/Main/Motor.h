#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Encoder.h>
#include <IntervalTimer.h>

class Motor {
public:
    Motor(int in1, int in2, int encA, int encB);
    
    void updateRPM();  
    float getRPM();
    void setTargetRPM(float rpm);
    void updateControl();
    int getEncoderPosition();

private:
    int in1, in2;
    Encoder encoder;
    float currentRPM;
    float targetRPM;
    int pwmValue;
    unsigned long lastUpdateTime;
    int lastPosition;
};

// **Deklariere die globalen Timer-Variablen hier**
extern IntervalTimer motorTimer;
extern Motor* globalMotor;
void motorTimerCallback();

#endif // MOTOR_H
