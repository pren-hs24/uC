#pragma once
#include <QTRSensors.h>

class LineSensor {
public:
  void begin(const uint8_t* pins, uint8_t numSensors);  // Sensoren initialisieren
  uint16_t readPosition();       // Linienposition (0–10000)
  bool isOnLine();
  bool isOnLine1();                // Linie erkannt?
  bool onPoint();                // Linie unter allen Sensoren?
  uint16_t* getSensorValues();   // Rohwerte zurückgeben
  

private:
  QTRSensors qtr;
  uint16_t sensorValues[8];      // Max. 8 Sensoren unterstützt
  uint8_t sensorCount;
 
};