#ifndef _MY_TIMER_H
#define _MY_TIMER_H
#include "Config.h"

#define PAST_TIME 1603579255
#define START_TIME 1000000000
#define GMT_OFFSET 3600
class MyTimer  {
public:
  MyTimer();
  void init(int);
  uint32_t tick();
  bool getTimeString(char *buf, int bufsize);
  bool getTimeStringShort(char *buf, int bufsize);
  void getTimeString(char *buf, int bufsize, uint32_t the_time);
  int getMinsOfDay();
  bool isValid();
  bool checkTimes();
  bool update();
  
private:
  uint32_t ntp_unix_time;
  bool ntp_is_valid;
  int delta_hours;
  uint32_t getNTP();
};

#endif