#include "UART.h"

int8_t speedValue = 0;  // Standardwert = 0

// interne Variable für das zuletzt empfangene Kommando
static UART_Command lastCommand = {
    .valid = false,
    .command = CMD_SET_SPEED,
    .payload = {0},
    .payloadLength = 0
};


// Initialisiert die UART-Schnittstelle (Serial1 für Raspberry Pi)
void UART_Init() {
  Serial.begin(115200);   // USB-Serial für Debugging
  Serial1.begin(115200);  // UART1 mit 8 Datenbits, keine Parität, 1 Stoppbit
  Serial.println("Teensy UART (Serial1) Ready");
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

  // Kopf: Eventcode
  message[0] = eventCode;

  // Nur kopieren, wenn payload existiert und Länge > 0
  if (payload != nullptr && payloadLength > 0) {
    memcpy(&message[1], payload, payloadLength);
  }

  // Checksumme über [eventCode + payload]
  message[payloadLength + 1] = UART_CalculateChecksum(message, payloadLength + 1);

  // Gesamtnachricht senden (eventCode + payload + checksum)
  Serial1.write(message, payloadLength + 2);
}

void UART_LogMessage(const char* fmt, ...) {
  char buffer[64];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  uint8_t len = strlen(buffer);
  uint8_t payload[1 + len];
  payload[0] = len;
  memcpy(&payload[1], buffer, len);

  UART_SendEvent(EVT_LOG_MESSAGE, payload, sizeof(payload));
}



// Sendet das "Point Reached" Event
void UART_SendPointReachedEvent() {
  uint8_t message[2] = { EVT_POINT_REACHED, UART_CalculateChecksum(message, 1) };
  Serial1.write(message, sizeof(message));  // Über Serial1 senden
}

// Empfängt und verarbeitet UART-Nachrichten von Serial1
void UART_Receive() {
    static uint8_t buffer[32];
    static size_t index = 0;

    while (Serial1.available()) {
        uint8_t receivedByte = Serial1.read();
        buffer[index++] = receivedByte;

        // Mindestgröße erreicht: [Command]...[Payload]...[Checksum]
        if (index > 1) {
            uint8_t checksum = UART_CalculateChecksum(buffer, index - 1);
            if (checksum == buffer[index - 1]) {
                // Checksumme OK – Befehl speichern
                lastCommand.command = static_cast<CommandCode>(buffer[0]);
                lastCommand.payloadLength = index - 2;  // ohne Command und Checksumme
                memcpy(lastCommand.payload, &buffer[1], lastCommand.payloadLength);
                lastCommand.valid = true;

                // Debug
                Serial.print("Kommando empfangen: 0x");
                Serial.println(lastCommand.command, HEX);
                Serial.print("Payload-Länge: ");
                Serial.println(lastCommand.payloadLength);

                // Puffer zurücksetzen
                index = 0;
            }
        }

        if (index >= sizeof(buffer)) {
            Serial.println("Pufferüberlauf – reset");
            index = 0;
        }
    }
}


const UART_Command* UART_GetLastCommand() {
    return &lastCommand;
}

void UART_ClearLastCommand() {
    lastCommand.valid = false;
}

