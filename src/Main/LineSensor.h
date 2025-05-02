#pragma once
#include <QTRSensors.h>

#define NUM_SENSORS 10

class LineSensor {
public:
  void begin();                   // Initialisiert Sensoren & kalibriert
  void loopDebug();              // Gibt Sensorwerte live über Serial aus
  uint16_t readPosition();       // Gibt Linienposition zurück (0–10000)
  bool isOnLine();               // Gibt true zurück, wenn eine Linie erkannt wurde
  uint16_t* getSensorValues();
  bool onPoint();
private:
  QTRSensors qtr;
  uint16_t sensorValues[NUM_SENSORS];
};