#ifndef UART_H
#define UART_H

#include <Arduino.h>

// UART Einstellungen
#define UART_BAUDRATE 115200

// Enum für Command-Codes (Raspberry Pi → Teensy)
enum CommandCode : uint8_t {
    CMD_TURN = 0x01,
    CMD_FOLLOW_LINE = 0x02,
    CMD_DEST_REACHED = 0x03,
    CMD_SET_DEBUG = 0x04,
    CMD_SET_SPEED = 0x05
};

// Enum für Event-Codes (Teensy → Raspberry Pi)
enum EventCode : uint8_t {
    EVT_START = 0x10,
    EVT_POINT_REACHED = 0x11,
    EVT_NO_LINE_FOUND = 0x12,
    EVT_NEXT_POINT_BLOCKED = 0x13,
    EVT_OBSTACLE_DETECTED = 0x14,
    EVT_ALIGNED = 0x15,
    EVT_RETURNING = 0x16,
    EVT_LOG_MESSAGE = 0x17
};

// Globale Variable für Geschwindigkeit
extern int8_t speedValue;

// Getter-Funktion für Speed-Wert
int8_t UART_GetSpeed();

// Initialisiert die UART-Schnittstelle auf Serial1
void UART_Init();

// Prüft, ob eine Nachricht empfangen wurde, und verarbeitet sie
void UART_Receive();

// Sendet ein Event über UART (Serial1)
void UART_SendEvent(EventCode eventCode, uint8_t *payload, size_t payloadLength);

//Hilfsfunktion zum Versenden von Log messages
void UART_LogMessage(const char* fmt, ...) 

// Sendet das "Point Reached" Event
void UART_SendPointReachedEvent();

// Maximal erlaubte Payload-Länge
#define UART_MAX_PAYLOAD 30

// Struktur zur Speicherung des zuletzt empfangenen Kommandos
struct UART_Command {
    bool valid;                   // Wurde ein vollständiges Kommando empfangen?
    CommandCode command;          // Kommando-Typ
    uint8_t payload[UART_MAX_PAYLOAD];  // Payload-Daten
    size_t payloadLength;         // Länge der Payload
};

// Zugriff auf das zuletzt empfangene Kommando
const UART_Command* UART_GetLastCommand();

// Reset, nachdem das Kommando verarbeitet wurde
void UART_ClearLastCommand();

#endif
