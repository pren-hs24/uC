#include <Wire.h>
#include "config.h"
#include "Motor.h"
#include "UART.h"

uint8_t debug = 0;

void setup() {
  //for debugging purposes
  Serial.begin(115200);

  //pinMux für Schalter
  pinMode(START, INPUT);
  pinMode(GOAL_A, INPUT);
  pinMode(GOAL_B, INPUT);
  pinMode(GOAL_C, INPUT);

  //Init Buzzer
  pinMode(BUZZER, OUTPUT);

  UART_Init();
}

void loop() {
  //wait until  start button is pressed
  while (!digitalRead(START))
    ;
  uint8_t payload[] = { TARGET_A };
  if (digitalRead(GOAL_A)) {
    payload[0] = TARGET_A;
  } else if (digitalRead(GOAL_B)) {
    payload[0] = TARGET_B;
  } else if (digitalRead(GOAL_C)) {
    payload[0] = TARGET_C;
  } else {
    Serial.println("Fehler bei Auswertung Zielschalter");
  }
  UART_SendEvent(EVT_START, payload, sizeof(payload));
}
