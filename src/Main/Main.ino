#include "Motor.h"
#include "UART.h"

#define IN1 3  //Pin for IN1 on H-Bridge1
#define IN2 4  //Pin for IN2 on H-Bridge1

#define PHASEA 10  //Phase A of encoder 1
#define PHASEB 11  //Phase B of encoder 1

#define MAX_RPM 240

Motor motor(IN1, IN2, PHASEA, PHASEB);  // Pins für Motorsteuerung und Encoder

int8_t last_speed = 0;

void setup() {
  UART_Init();

  globalMotor = &motor;  // Motor-Instanz für den Timer setzen

  // Starte den Timer mit einer Abtastzeit von 10ms (100 Hz)
  motorTimer.begin(motorTimerCallback, 10000);  // 10000 Mikrosekunden = 10 ms
}

void loop() {
  UART_Receive();
  int8_t current_speed = UART_GetSpeed();

  if (current_speed != last_speed) {
    last_speed = current_speed;
    float rpm = MAX_RPM * (current_speed / 100.0);
    Serial.print("Setze rpm: ");
    Serial.println(rpm);
    motor.setTargetRPM(rpm);

    // Nachricht mit Text + Speed-Wert senden
    uint8_t payload[20];  // Array für Nachricht + Speed
    const char *msg = "RPM Motor set to: ";
    size_t msgLen = strlen(msg);

    memcpy(payload, msg, msgLen);    // Text kopieren
    payload[msgLen] = rpm;  // Speed-Wert anhängen

    UART_SendEvent(EVT_LOG_MESSAGE, payload, msgLen + 1);
  }
}