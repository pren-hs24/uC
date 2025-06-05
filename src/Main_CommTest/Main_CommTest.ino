#include <Wire.h>
#include "config.h"
#include "Motor.h"
#include "UART.h"
#include "FahrBefehle.h"
#include "TOF.h"
#include "Turn.h"
#include "Melody.h"

//Motoren und Encoder initalisieren
Motor motorLeft(IN1_L, IN2_L, PHASE_A_L, PHASE_B_L);
Motor motorRight(IN1_R, IN2_R, PHASE_A_R, PHASE_B_R);

IntervalTimer timerRight;
IntervalTimer timerLeft;
IntervalTimer timerMotors;
IntervalTimer timerLine;

//ToF Sensoren instanzieren
TOF tof_front_upper(addr_f_upper, XSHUT_TOF_1);
TOF tof_front_lower(addr_f_lower, XSHUT_TOF_2);
TOF tof_back(addr_b, XSHUT_TOF_3);

//Servo
ServoMotor myServo(SERVO);
//Endschalter
Endschalter endschalter(ENDSCHALTER_PIN);
//Liniensensoren
LineSensor frontSensor;
LineSensor backSensor;
//Konstanten
uint8_t debug = 0;
bool finished = false;
//Funktions Deklarationen
void turn(uint16_t angle, uint8_t snap);
void follow(void);
void destinationReached(void);
void setDebugLogging(uint8_t enabled);
void setSpeed(int8_t speed);
void updateMotors(void);
void lineControl(void);

//Setup code
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
  //Endschalter
  endschalter.begin();

  //Servomotor
  myServo.begin();

  UART_Init();

  motorLeft.attachTimer(&timerLeft);
  motorRight.attachTimer(&timerRight);
  timerMotors.begin(updateMotors, 10000);

  WIRE_BUS.begin();
  WIRE_BUS.setClock(100000);

  tof_front_upper.setAddress();
  tof_front_lower.setAddress();
  tof_back.setAddress();

  tof_front_upper.init();
  tof_front_lower.init();
  tof_back.init();

  frontSensor.begin(frontPins, 5);
  backSensor.begin(backPins, 5);

  //lineFollow_init(motorRight,motorLeft,tof_front_lower,tof_back);
}

void loop() {
  //wait until  start button is pressed
  finished = false;
  while (!digitalRead(START))
    ;
  delay(100);
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
  Serial.printf("Event Start mit Ziel %i an Rapsi gesendet.\n", payload[0]);
  UART_SendEvent(EVT_START, payload, sizeof(payload));
  while (!finished) {
    UART_Receive();

    const UART_Command* cmd = UART_GetLastCommand();
    if (cmd->valid) {
      UART_ClearLastCommand();  //Kommando gültig und wird verarbeitet
      switch (cmd->command) {
        case CMD_TURN:
          Serial.printf("Dreh Befehl erhalten\n");
          if (cmd->payloadLength >= 3) {
            int16_t angle = cmd->payload[0] | (cmd->payload[1] << 8);
            uint8_t snap = cmd->payload[2];
            Serial.printf("angle: %i | snap: %i");
            turn(angle, snap);
          }
          break;
        case CMD_FOLLOW_LINE:
          followLine(40);
          break;
        case CMD_DEST_REACHED:
          destinationReached();
          break;
        case CMD_SET_DEBUG:
          if (cmd->payloadLength >= 1) {
            uint8_t enabled = static_cast<uint8_t>(cmd->payload[0]);
            setDebugLogging(enabled);
          }
          break;
        case CMD_SET_SPEED:
          if (cmd->payloadLength >= 1) {
            int8_t speed = static_cast<int8_t>(cmd->payload[0]);
            setSpeed(speed);
          }
          break;
      }
    }
    delay(100);
  }
}

//dummy funktion für Drehung
void turn(int16_t angle, uint8_t snap) {
  Serial.printf("Drehe UFO um %i Grad", angle);
  if(debug){
    UART_LogMessage("Drehe UFO um %i Grad", angle)
  }
  Turn_UFO((float)angle);
  uint8_t message[] = { 0 };
  if(snap&&(!frontSensor.isOnLine())){
    message[0]=1;
  }
  UART_SendEvent(EVT_ALIGNED, message, sizeof(message));
}
//funktion für Linienverfolgung
void follow() {
  while(!followLine(DEFAULT_SPEED)) delay(5);
  UART_SendEvent(EVT_POINT_REACHED, nullptr, 0);
}
//beinhaltet Logik, wenn Ziel erreicht wurde
void destinationReached() {
  playMelody();
  finished = true;
  //play finishing melody
}
//Funktion um debuging zu Raspi zu senden
void setDebugLogging(uint8_t enabled) {
  debug = enabled;
}
//Setzt Rad drehzahl -> wird nur für Tests benötigt
void setSpeed(int8_t speed) {
  float calc_speed = ((float)speed / 100) * MAX_SPEED;
  Serial.printf("Setze RPM: %.2f\n", calc_speed);
  if(debug){
    UART_LogMessage("Setze RPM: %.2f\n", calc_speed);
  }
  motorLeft.setTargetRPM(calc_speed);
  motorRight.setTargetRPM(-calc_speed);
}

void updateMotors() {
  motorLeft.updateRPM();
  motorRight.updateRPM();
  motorLeft.updateControl();
  motorRight.updateControl();
}