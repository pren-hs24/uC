#include "Motor.h"

// Pinbelegung linker Motor
#define IN1_LEFT 3
#define IN2_LEFT 4
#define ENC_A_LEFT 10
#define ENC_B_LEFT 11

// Pinbelegung rechter Motor
#define IN1_RIGHT 5
#define IN2_RIGHT 6
#define ENC_A_RIGHT 12
#define ENC_B_RIGHT 13

// Motorobjekte
Motor motorLeft(IN1_LEFT, IN2_LEFT, ENC_A_LEFT, ENC_B_LEFT);
Motor motorRight(IN1_RIGHT, IN2_RIGHT, ENC_A_RIGHT, ENC_B_RIGHT);

// Timer
IntervalTimer pwmTimerLeft;
IntervalTimer pwmTimerRight;
IntervalTimer controlTimer;

// Regel-Callback (100 Hz)
void controlCallback() {
  motorLeft.updateRPM();
  motorLeft.updateControl();

  motorRight.updateRPM();
  motorRight.updateControl();

  // Serielle Ausgabe (optional)
  Serial.print("L: ");
  Serial.print(motorLeft.getRPM());
  Serial.print(" RPM | R: ");
  Serial.print(motorRight.getRPM());
  Serial.println(" RPM");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Starte zwei-Motoren-Test...");

  // Timer zuweisen
  motorLeft.attachTimer(&pwmTimerLeft);
  motorRight.attachTimer(&pwmTimerRight);

  // Regel-Timer starten (10ms = 100 Hz)
  controlTimer.begin(controlCallback, 10000);
}

void loop() {
  // === 1. Geradeausfahrt ===
  Serial.println("Beide Motoren: Vorwärts 100 RPM");
  motorLeft.setTargetRPM(100);
  motorRight.setTargetRPM(-100);
  delay(5000);

  // === 2. Stopp ===
  Serial.println("Stop");
  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(3000);

  // === 3. Linkskurve (nur rechter Motor läuft) ===
  Serial.println("Kurve: nur rechter Motor");
  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(-100);
  delay(3000);

  // === 4. Rückwärtsfahrt ===
  Serial.println("Beide Motoren: Rückwärts -80 RPM");
  motorLeft.setTargetRPM(-80);
  motorRight.setTargetRPM(80);
  delay(5000);

  // === 5. Stopp ===
  motorLeft.setTargetRPM(0);
  motorRight.setTargetRPM(0);
  delay(3000);
}
