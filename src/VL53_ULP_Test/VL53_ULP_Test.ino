#include <Arduino.h>
#include <Wire.h>
#include "TOF.h"

//I2C BUS
#define Wire_Bus Wire2
// #define I2C_addr_1 0x30
// #define I2C_addr_2 0x31
#define XSHUT_2 38
#define XSHUT_3 39

const uint16_t addr_1 = 0x30;
const uint16_t addr_2 = 0x31;

TOF tof_front(addr_1, XSHUT_2);
TOF tof_back(addr_2, XSHUT_3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starte serielle kommunikation");

  Wire_Bus.begin();
  Wire_Bus.setClock(100000);

  Serial.println("VL53L3CX Initialisierung...");

  if (!tof_front.setAddress()) {
    Serial.println("Fehler bei setzen der Addresse von ToF front");
  } else {
    Serial.println("Tof front Addresse erfolgreich gesetzt");
  }

  if (!tof_back.setAddress()) {
    Serial.println("Fehler bei setzen der Addresse von ToF back");
  } else {
    Serial.println("Tof back Addresse erfolgreich gesetzt");
  }

  if (!tof_front.init()) {
    Serial.println("Fehler in Init von ToF front");
    while(1);
  } else {
    Serial.println("Tof front init erfolgreich");
  }

  if (!tof_back.init()) {
    Serial.println("Fehler in Init von ToF back");
  } else {
    Serial.println("Tof back init erfolgreich");
  }
}

void loop() {
  uint16_t distance_1 = 0;
  uint16_t distance_2 = 0;

  if (!tof_front.meassure(&distance_1)) {
    Serial.println("Fehler in Messung ToF Front");
  }
  if (!tof_back.meassure(&distance_2)) {
    Serial.println("Fehler in Messung TOF Back");
  }

  Serial.printf("Distanz ToF Front: %i mm\n", distance_1);
  Serial.printf("Distanz ToF Back: %i mm\n", distance_2);

  delay(100);
}
