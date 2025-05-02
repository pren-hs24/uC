#include <Servo.h>

class ServoMotor {
public:
  ServoMotor(int pin);
  void begin();
  void moveTo(int angle);
  void liftHindernis();
  void dropHindernis();

private:
  int _currentAngle;
  int _pin;
  Servo _servo;
};