#ifndef _BATTY_H
#define _BATTY_H
#include <Arduino.h>
#include "Config.h"

extern float batt_voltage;
extern uint8_t batt_level;

class Batty {
public:
  Batty();
  void init();
  void read();
  void test();
private:
  uint8_t level();
  int vref;
};

#endif
