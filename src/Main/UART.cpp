#include "UART.h"

int8_t speedValue = 0;  // Standardwert = 0

// Initialisiert die UART-Schnittstelle (Serial1 für Raspberry Pi)
void UART_Init() {
  Serial.begin(115200);   // USB-Serial für Debugging
  Serial1.begin(115200);  // UART1 mit 8 Datenbits, keine Parität, 1 Stoppbit
  Serial.println("Teensy UART (Serial1) Ready");
  pinMode(DEBUGG_LED, OUTPUT);
}

int8_t UART_GetSpeed() {
  return speedValue;
}


// Berechnet die Checksumme (XOR aller Bytes)
uint8_t UART_CalculateChecksum(uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < length; i++) {
    checksum ^= data[i];
  }
  return checksum;
}

// Sendet ein Event über Serial1 (Teensy → Raspberry Pi)
void UART_SendEvent(EventCode eventCode, uint8_t *payload, size_t payloadLength) {
  uint8_t message[32];
  message[0] = eventCode;
  memcpy(&message[1], payload, payloadLength);
  message[payloadLength + 1] = UART_CalculateChecksum(message, payloadLength + 1);

  Serial1.write(message, payloadLength + 2);  // Über Serial1 senden
}

// Sendet das "Point Reached" Event
void UART_SendPointReachedEvent() {
  uint8_t message[2] = { EVT_POINT_REACHED, UART_CalculateChecksum(message, 1) };
  Serial1.write(message, sizeof(message));  // Über Serial1 senden
}

// Empfängt und verarbeitet UART-Nachrichten von Serial1
void UART_Receive() {
  static uint8_t buffer[32];  // Puffer für empfangene Daten
  static size_t index = 0;    // Aktuelle Position im Puffer

  while (Serial1.available()) {
    uint8_t receivedByte = Serial1.read();  // Byte lesen
    buffer[index++] = receivedByte;         // In Puffer speichern

    // // Debugging: Jedes Byte ausgeben
    // Serial.print("Empfangenes Byte: 0x");
    // Serial.println(receivedByte, HEX);

    // Prüfen, ob die Nachricht vollständig ist (mindestens 2 Bytes: Command + Checksum)
    if (index > 1) {
      uint8_t checksum = UART_CalculateChecksum(buffer, index - 1);
      if (checksum == buffer[index - 1]) {
        //Serial.println("Nachricht vollständig & Checksumme OK!");

        CommandCode command = static_cast<CommandCode>(buffer[0]);

        Serial.print("Befehl erkannt: 0x");
        Serial.println(command, HEX);

        switch (command) {
          case CMD_TURN:
            {
              int16_t angle = buffer[1] | (buffer[2] << 8);
              uint8_t snap = buffer[3];
              Serial.print("Drehe um: ");
              Serial.print(angle);
              Serial.print("° mit Snap: ");
              Serial.println(snap);
              break;
            }
          case CMD_FOLLOW_LINE:
            Serial.println("Folge Linie!");
            break;
          case CMD_SET_SPEED:
            {
              int8_t speed = buffer[1];
              speedValue = speed;
              Serial.print("Setze Geschwindigkeit auf: ");
              Serial.println(speed);
              break;
            }
          case CMD_SET_DEBUG:
            {
              Serial.println("Debug logging wird asugegeben!");
              break;
            }
          default:
            Serial.println("Unbekannter Befehl!");
            break;
        }

        // Puffer zurücksetzen
        index = 0;
      }
    }

    // Falls der Puffer zu groß wird (z. B. ungültige Nachricht), zurücksetzen
    if (index >= sizeof(buffer)) {
      Serial.println("Fehler: Nachricht zu lang! Puffer zurückgesetzt.");
      index = 0;
    }
  }
}
