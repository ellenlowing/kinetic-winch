#include "Winch.h"

Winch::Winch(int en, int step, int dir, int ms1, int ms2, int ms3) {
  _en = en;
  _step = step;
  _dir = dir;
  _ms1 = ms1;
  _ms2 = ms2;
  _ms3 = ms3;
  pinMode(_en, OUTPUT);
  pinMode(_step, OUTPUT);
  pinMode(_dir, OUTPUT);
  pinMode(_ms1, OUTPUT);
  pinMode(_ms2, OUTPUT);
  pinMode(_ms3, OUTPUT);
}

/**
    Sets direction of rotation of motor.

    @param unwind If unwinding cable, set HIGH, else set LOW
*/
void Winch::setDirection(bool unwind) {
  digitalWrite(_dir, unwind);
}

/**
    Sets speed of rotation of motor.

    @param rpm Rotation per minute
*/
void Winch::setSpeed(int rpm) {
  _rpm = rpm;
  _delay_us = rpm 
}

/**
    Sets stepping resolution of motor.

    @param fract The fraction of one full step
*/
void Winch::setResolution(int fract) {
  _step_mode = fract;
  switch(fract) {
    case 1:
      digitalWrite(_ms1, LOW);
      digitalWrite(_ms2, LOW);
      digitalWrite(_ms3, LOW);
      break;

    case 2:
      digitalWrite(_ms1, HIGH);
      digitalWrite(_ms2, LOW);
      digitalWrite(_ms3, LOW);
      break;

    case 4:
      digitalWrite(_ms1, LOW);
      digitalWrite(_ms2, HIGH);
      digitalWrite(_ms3, LOW);
      break;

    case 8:
      digitalWrite(_ms1, HIGH);
      digitalWrite(_ms2, HIGH);
      digitalWrite(_ms3, LOW);
      break;

    case 16:
      digitalWrite(_ms1, HIGH);
      digitalWrite(_ms2, HIGH);
      digitalWrite(_ms3, HIGH);
      break;

    default:
      digitalWrite(_ms1, LOW);
      digitalWrite(_ms2, LOW);
      digitalWrite(_ms3, LOW);
  }
}

/**
    Drives motor to take one full step.
*/
void Winch::step() {
  for(int i = 0; i < _step_mode; i++) {
    microstep();
  }
}

/**
    Drives motor with given angle in degrees.

    @param angle Rotation angle in degrees
*/
void Winch::rotate(int angle) {
  int steps = angle / 360 * STEPS_PER_REV * _step_mode;
  for(int i = 0; i < steps; i++) {
    microstep();
  }
}

/**
    Drives motor to take one micro step.
*/
void Winch::microstep() {
  digitalWrite(_step, HIGH);
  delayMicroseconds(_delay_us); // use speed param
  digitalWrite(_step, LOW);
  delayMicroseconds(_delay_us); // same as above
}
