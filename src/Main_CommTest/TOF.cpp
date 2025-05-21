#include "TOF.h"
#include <Wire.h>

const uint16_t std_addr = 0x29;

TOF::TOF(uint16_t addr, uint8_t xshut)
  : addr(addr), xshut(xshut) {

  pinMode(xshut, OUTPUT);
  digitalWrite(xshut, LOW);
  delay(10);
}

bool TOF::setAddress() {
  digitalWrite(xshut, HIGH);
  delay(100);
  
  // uint8_t status = VL53L3CX_ULP_WaitForBoot(std_addr);

  // if(status != VL53L3CX_ULP_ERROR_NONE){
  //   Serial.println("Fehler nach wait for boot");
  //   Serial.print("Boot state bei Fehler: ");
  //   Serial.println(status, HEX);
  //   return false;
  // }

  // Wire2.beginTransmission((int)std_addr);
  // uint8_t error = Wire2.endTransmission();
  // if (error != 0) {
  //   Serial.println("Sensor antwortet nicht auf 0x29 nach XSHUT HIGH!");
  //   return false;
  // }

  if (VL53L3CX_ULP_SetI2CAddress(std_addr, (uint8_t)addr) != VL53L3CX_ULP_ERROR_NONE) {
    return false;
  }
  Serial.print("I2C gesetzt auf ");
  Serial.println(addr, HEX);

  //delay(10);  // neue Pause nach Adressänderung
  return true;
}

bool TOF::init() {
  // Sensor initialisieren
  // if(VL53L3CX_ULP_WaitForBoot(addr)!= VL53L3CX_ULP_ERROR_NONE){
  //   Serial.println("Fehler nach wait for boot");
  //   return false;
  // }
  // Wire2.beginTransmission((int)addr);
  // uint8_t error = Wire2.endTransmission();
  // if (error != 0) {
  //   Serial.println("Sensor antwortet nicht auf neue Addresse nach wechsel!");
  //   return false;
  // }
  if (VL53L3CX_ULP_SensorInit(addr) != VL53L3CX_ULP_ERROR_NONE) {
    Serial.println("Init failed");
    return false;
  }
  // // Optimierungen für 3 Meter Ziel
  // if (VL53L3CX_ULP_SetMacroTiming(addr, 300) != VL53L3CX_ULP_ERROR_NONE) {
  //   Serial.println("Fail in setting macro timing");
  //   return false;
  // }  // lange Integration
  // if (VL53L3CX_ULP_SetSignalThreshold(addr, 500) != VL53L3CX_ULP_ERROR_NONE) {
  //   Serial.println("Setting of signal threshold failed");
  //   return false;
  // }  // niedriger Schwellwert
  // if (VL53L3CX_ULP_SetInterMeasurementInMs(addr, 350) != VL53L3CX_ULP_ERROR_NONE) {
  //   Serial.println("Setting intMeas failed");
  //   return false;
  // }  // IM > Macro Timing

  Serial.print("VL53L3CX bereit an Addresse: ");
  Serial.println(addr, HEX);
  return true;
}

bool TOF::meassure(uint16_t *distance) {
  uint8_t data_ready = 0;
  uint8_t status;
  uint8_t measurement_status;
  uint16_t sigma_mm;
  uint16_t signal_kcps;
  uint16_t ambient_kcps;

  // Interrupt löschen
  VL53L3CX_ULP_ClearInterrupt(addr);

  // Messung starten
  status = VL53L3CX_ULP_StartRangingSingleShot(addr);
  if (status != VL53L3CX_ULP_ERROR_NONE) {
    Serial.printf("Fehler bei Messstart mit Errorcode: %i\n", status);
    return false;
  }

  // Auf Messung warten
  do {
    VL53L3CX_ULP_CheckForDataReady(addr, &data_ready);
    delay(1);
  } while (data_ready == 0);
  Serial.println("Messdaten Bereit!");

  // Debugdaten lesen
  status = VL53L3CX_ULP_DumpDebugData(
    addr,
    &measurement_status,
    distance,
    &sigma_mm,
    &signal_kcps,
    &ambient_kcps);

  if (status != VL53L3CX_ULP_ERROR_NONE) {
    return false;
  }

  if (measurement_status != 0) {
    *distance = -1;
  }

  return true;
}