#include "Endschalter.h"

Endschalter::Endschalter(uint8_t pin) : pin_(pin) {}

void Endschalter::begin() {
  pinMode(pin_, INPUT);  // Externer Pull-Down, daher kein INPUT_PULLUP
}

bool Endschalter::isPressed() {
  return digitalRead(pin_) == HIGH;  // gedrückt = HIGH
}

