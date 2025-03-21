#include "Motor.h"

IntervalTimer motorTimer;      // Hardware-Timer
Motor* globalMotor = nullptr;  // Globale Motor-Instanz, Standard auf nullptr

void motorTimerCallback() {
  if (globalMotor) {
    globalMotor->updateRPM();
    globalMotor->updateControl();
  }
}
const int ticksPerRevolution = 2184;  // Korrigierte Encoder-Auflösung
unsigned int long current_rpmTime = 0;

Motor::Motor(int in1, int in2, int encA, int encB)
  : in1(in1), in2(in2), encoder(encA, encB), currentRPM(0), targetRPM(0), pwmValue(0), lastUpdateTime(0), lastPosition(0) {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void Motor::updateRPM() {
  unsigned long currentTime = micros();             // Nutze micros() für µs-Auflösung
  float dt = (currentTime - lastUpdateTime) / 1e6;  // Zeit in Sekunden (10 µs = 0.00001 s)

  if (dt > 0.01) {  // Aktualisierung nur alle 10 µs
    int currentPosition = encoder.read();
    int deltaTicks = currentPosition - lastPosition;
    int ticksPerSecond = deltaTicks / dt;
    currentRPM = (ticksPerSecond / (float)ticksPerRevolution) * 60.0;

    lastPosition = currentPosition;
    lastUpdateTime = currentTime;
  }
}


float Motor::getRPM() {
  return currentRPM;
}

void Motor::setTargetRPM(float rpm) {
  targetRPM = rpm;
}

void Motor::updateControl() {
  static float integral = 0;   // Integrationsspeicher
  static float lastError = 0;  // Vorheriger Fehler für Differenzialanteil

  float error = targetRPM - currentRPM;  // Fehler berechnen

  // Angepasste PID-Werte
  float Kp = 0.0825;  // Niedrigere Verstärkung für sanftere Regelung
  float Ki = 0.0853;  // Langsamerer Integralaufbau
  float Kd = 0.0;     // Leichte Dämpfung

  integral += error;
  integral = constrain(integral, -3000, 3000);  // Begrenzung des Integrals

  float derivative = (error - lastError) / 0.01;  // Annäherung für Dämpfung
  lastError = error;

  float u_debug = Kp * error + Ki * integral;

  int pwmOutput = constrain(Kp * error + Ki * integral + Kd * derivative, -255, 255);  // Begrenzung auf gültigen PWM-Bereich

  // // PWM-Wert setzen
  // analogWrite(in1, pwmOutput);
  // digitalWrite(in2, LOW);  // Vorwärts fahren

  if(targetRPM==0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,HIGH);
  }
  //Vorwärts
  if (targetRPM >= 0) {
    digitalWrite(in1, HIGH);
    analogWrite(in2, 255 - pwmOutput);
  }
  //Rückwärts
  else if (targetRPM < 0) {
    digitalWrite(in2, HIGH);
    analogWrite(in1, 255 - abs(pwmOutput));
  }

  // Debugging
  Serial.print("Ziel: ");
  Serial.print(targetRPM);
  Serial.print(" RPM | Ist: ");
  Serial.print(currentRPM);
  Serial.print(" RPM | Fehler: ");
  Serial.print(error);
  Serial.print(" | Integral: ");
  Serial.print(integral);
  Serial.print(" | PWM: ");
  Serial.print(pwmOutput);
  Serial.print(" | Ki * Integral: ");
  Serial.println(Ki * integral);
  Serial.print(" | u: ");
  Serial.println(u_debug);
}

int Motor::getEncoderPosition() {
  return encoder.read();
}
