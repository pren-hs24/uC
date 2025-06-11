#include "LineSensor.h"
#include <Arduino.h>
#define BUZZER_PIN 7  // PWM-fähiger Piezoanschluss am Teensy

void LineSensor::begin(const uint8_t* pins, uint8_t numSensors) {
  sensorCount = numSensors;

  qtr.setTypeRC();
  qtr.setSensorPins(pins, numSensors);
 
  qtr.setTimeout(2500);
  delay(500);
  
  pinMode(BUZZER_PIN, OUTPUT);

 unsigned long start = millis();

  while (millis() - start < 4000) {
    qtr.calibrate();
    qtr.readCalibrated(sensorValues);
    for (uint8_t i = 0; i < numSensors; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');  // Tab als Trennung
    }
    Serial.println();
    delay(10);
  }
  for (uint8_t i = 0; i < numSensors; i++) {
  Serial.print(sensorValues[i]);
  Serial.print('\t');  // Tab als Trennung
  }
  
  // Abschluss: langer Piepton (500 ms, 2 kHz)
  analogWriteFrequency(BUZZER_PIN, 3000);
  analogWrite(BUZZER_PIN, 128);
  delay(500);
  analogWrite(BUZZER_PIN, 0);

}



uint16_t LineSensor::readPosition() {
  
  qtr.readCalibrated(sensorValues);
  return qtr.readLineWhite(sensorValues);
 
}

bool LineSensor::isOnLine() {
  qtr.readCalibrated(sensorValues);
  uint8_t activeCount = 0;

  for (uint8_t i = 0; i < sensorCount; i++) {
    if (sensorValues[i] < 200) {
      activeCount++;
      if (activeCount >= 2) {
        return true;  // Mindestens zwei Sensoren erkennen die Linie
      }
    }
  }

  return false;  // Weniger als zwei Sensoren aktiv → keine Linie
}

bool LineSensor::isOnLine1() {
  qtr.readCalibrated(sensorValues);

  uint8_t overThreshold = 0;
for (uint8_t i = 0; i < sensorCount; i++) {
  if (sensorValues[i] > 600) {
    overThreshold++;
  }
}
return overThreshold >= 5;  // true, wenn ALLE drüber sind
}

bool LineSensor::onPoint() {
  qtr.readCalibrated(sensorValues);

  uint8_t whiteCount = 0;

  for (uint8_t i = 0; i < sensorCount; i++) {
    if (sensorValues[i] < 200) {  // Sensor sieht Weiß
      whiteCount++;
    }
  }

  return whiteCount >= 3;
}