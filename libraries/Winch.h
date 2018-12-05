#ifndef Winch_h
#define Winch_h

#include "Arduino.h"
#define STEPS_PER_REV 200

class Winch
{
  public:
    Winch(int en, int step, int dir, int ms1, int ms2, int ms3);
    void setDirection(bool unwind);
    void setResolution(int fract);
    void step();
    void rotate(int angle);
    void microstep();
  private:
    int _en;
    int _step;
    int _dir;
    int _ms1;
    int _ms2;
    int _ms3;
    int _rpm;
    int _delay_us;
    int _step_mode;
};

#endif
