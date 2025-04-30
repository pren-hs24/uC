#include "VL53L3CX_ULP_platform.h"
#include <Arduino.h>
#include <Wire.h>  // Oder Wire1 / Wire2 je nach Bus!

// Wähle hier deinen I2C Bus:
#define VL53L3CX_WIRE_BUS Wire2

uint8_t VL53L3CX_ULP_RdByte(uint16_t dev, uint16_t registerAddr, uint8_t *value) {
    VL53L3CX_WIRE_BUS.beginTransmission(dev);
    VL53L3CX_WIRE_BUS.write((registerAddr >> 8) & 0xFF); // High Byte
    VL53L3CX_WIRE_BUS.write(registerAddr & 0xFF);         // Low Byte
    if (VL53L3CX_WIRE_BUS.endTransmission(false) != 0) { // repeated start
        return 1; // Fehler
    }

    if (VL53L3CX_WIRE_BUS.requestFrom(dev, (uint8_t)1) != 1) {
        return 1;
    }

    *value = VL53L3CX_WIRE_BUS.read();
    return 0; // Erfolg
}

uint8_t VL53L3CX_ULP_RdWord(uint16_t dev, uint16_t registerAddr, uint16_t *value) {
    uint8_t buffer[2];
    if (VL53L3CX_ULP_RdMulti(dev, registerAddr, buffer, 2) != 0) {
        return 1;
    }
    *value = (uint16_t)(buffer[0] << 8) | buffer[1];
    return 0;
}

uint8_t VL53L3CX_ULP_RdDWord(uint16_t dev, uint16_t registerAddr, uint32_t *value) {
    uint8_t buffer[4];
    if (VL53L3CX_ULP_RdMulti(dev, registerAddr, buffer, 4) != 0) {
        return 1;
    }
    *value = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) |
             ((uint32_t)buffer[2] << 8) | buffer[3];
    return 0;
}

uint8_t VL53L3CX_ULP_WrByte(uint16_t dev, uint16_t registerAddr, uint8_t value) {
    VL53L3CX_WIRE_BUS.beginTransmission(dev);
    VL53L3CX_WIRE_BUS.write((registerAddr >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(registerAddr & 0xFF);
    VL53L3CX_WIRE_BUS.write(value);
    if (VL53L3CX_WIRE_BUS.endTransmission() != 0) {
        return 1;
    }
    return 0;
}

uint8_t VL53L3CX_ULP_WrWord(uint16_t dev, uint16_t registerAddr, uint16_t value) {
    VL53L3CX_WIRE_BUS.beginTransmission(dev);
    VL53L3CX_WIRE_BUS.write((registerAddr >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(registerAddr & 0xFF);
    VL53L3CX_WIRE_BUS.write((value >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(value & 0xFF);
    if (VL53L3CX_WIRE_BUS.endTransmission() != 0) {
        return 1;
    }
    return 0;
}

uint8_t VL53L3CX_ULP_WrDWord(uint16_t dev, uint16_t registerAddr, uint32_t value) {
    VL53L3CX_WIRE_BUS.beginTransmission(dev);
    VL53L3CX_WIRE_BUS.write((registerAddr >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(registerAddr & 0xFF);
    VL53L3CX_WIRE_BUS.write((value >> 24) & 0xFF);
    VL53L3CX_WIRE_BUS.write((value >> 16) & 0xFF);
    VL53L3CX_WIRE_BUS.write((value >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(value & 0xFF);
    if (VL53L3CX_WIRE_BUS.endTransmission() != 0) {
        return 1;
    }
    return 0;
}

void VL53L3CX_ULP_WaitMs(uint32_t TimeMs) {
    delay(TimeMs);
}

// Hilfsfunktion: Lesen von mehreren Bytes (intern genutzt)
uint8_t VL53L3CX_ULP_RdMulti(uint16_t dev, uint16_t registerAddr, uint8_t* pdata, uint16_t count) {
    VL53L3CX_WIRE_BUS.beginTransmission(dev);
    VL53L3CX_WIRE_BUS.write((registerAddr >> 8) & 0xFF);
    VL53L3CX_WIRE_BUS.write(registerAddr & 0xFF);
    if (VL53L3CX_WIRE_BUS.endTransmission(false) != 0) { // Repeated start
        return 1;
    }

    uint16_t idx = 0;
    VL53L3CX_WIRE_BUS.requestFrom(dev, (uint8_t)count);
    while (VL53L3CX_WIRE_BUS.available() && idx < count) {
        pdata[idx++] = VL53L3CX_WIRE_BUS.read();
    }

    if (idx != count) {
        return 1;
    }

    return 0;
}
