#ifndef TOF_H_
#define TOF_H_

#include <Arduino.h>
#include "VL53L3CX_ULP_api.h"
#include "VL53L3CX_ULP_platform.h"

class TOF{
  public:
  TOF(uint16_t addr , uint8_t xshut);

  bool setAddress();
  bool init();

  bool meassure(uint16_t *distance);

  private:
  uint16_t addr;
  uint8_t xshut;
};

#endif