#ifndef _VL53L3CX_ULP_PLATFORM_H_
#define _VL53L3CX_ULP_PLATFORM_H_
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t VL53L3CX_ULP_RdDWord(uint16_t dev, uint16_t registerAddr, uint32_t *value);
uint8_t VL53L3CX_ULP_RdWord(uint16_t dev, uint16_t registerAddr, uint16_t *value);
uint8_t VL53L3CX_ULP_RdByte(uint16_t dev, uint16_t registerAddr, uint8_t *value);

uint8_t VL53L3CX_ULP_WrByte(uint16_t dev, uint16_t registerAddr, uint8_t value);
uint8_t VL53L3CX_ULP_WrWord(uint16_t dev, uint16_t registerAddr, uint16_t value);
uint8_t VL53L3CX_ULP_WrDWord(uint16_t dev, uint16_t registerAddr, uint32_t value);

uint8_t VL53L3CX_ULP_RdMulti(uint16_t dev, uint16_t registerAddr, uint8_t* pdata, uint16_t count);

void VL53L3CX_ULP_WaitMs(uint32_t TimeMs);

#ifdef __cplusplus
}
#endif

#endif // _VL53L3CX_ULP_PLATFORM_H_
