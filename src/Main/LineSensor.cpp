#include "LineSensor.h"
#include <Arduino.h>
#define BUZZER_PIN 7  // PWM-fähiger Piezoanschluss am Teensy

void LineSensor::begin() {
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24}, NUM_SENSORS);
  qtr.setTimeout(2500);
  delay(500);
  
  pinMode(BUZZER_PIN, OUTPUT);

 unsigned long start = millis();

  while (millis() - start < 10000) {
    qtr.calibrate();
    qtr.readCalibrated(sensorValues);
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');  // Tab als Trennung
    }
    Serial.println();
    delay(10);
  }
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
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

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] < 600) {
      // Mindestens ein Sensor sieht die Linie
      return true;
    }
  }

  // Kein Sensor sieht die Linie
  return false;
}


bool LineSensor::onPoint() {
  qtr.readCalibrated(sensorValues);

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] > 500) {
      // Mindestens ein Sensor sieht nicht weiß
      return false;
    }
  }

  // Alle Sensoren sehen weiß
  return true;
}
