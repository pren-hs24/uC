#pragma once
#include <Encoder.h>
#include <IntervalTimer.h>

class Motor {
  public:
    Motor(int in1, int in2, int encA, int encB);

    void updateRPM();
    void updateControl();
    void setTargetRPM(float rpm);
    float getRPM();
    int getEncoderPosition();
    int getDirection();  // Fahrtrichtung abrufen
    volatile int pwmCounter;   // zählt 0 bis 255 (PWM-Phase)
    volatile int pwmOnTime;    // wie viele Zyklen der Motor EIN ist

    void pwmISR();  // ISR-Funktion pro Instanz
    static void pwmISRWrapper0();  // Wrapper für Motor 0
    static void pwmISRWrapper1();  // Wrapper für Motor 1

    void attachTimer(IntervalTimer* t);  // Timer-Referenz registrieren
    void setPWMFromSensor(int pwm, int direction);

    friend void pwmISRWrapper0();
    friend void pwmISRWrapper1();

  private:
    int in1, in2;
    Encoder encoder;
    float currentRPM;
    float targetRPM;
    unsigned long lastUpdateTime;
    int lastPosition;
    
    int pwmDuty;
    int direction;
  
    IntervalTimer* pwmTimer;

    static Motor* instance0;
    static Motor* instance1;
};
