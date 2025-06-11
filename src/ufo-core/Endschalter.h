#pragma once
#include <Arduino.h>

class Endschalter {
public:
  Endschalter(uint8_t pin);
  void begin();
  bool isPressed();  // true, wenn gedrückt (HIGH)
  
private:
  uint8_t pin_;
};