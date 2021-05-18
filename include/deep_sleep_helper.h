#ifndef _HELP_STUFF_H
#define _HELP_STUFF_H
#include <Arduino.h>

#define SHORT_TIMEOUT  0
#define MEDIUM_TIMEOUT 1
#define LONG_TIMEOUT   2

extern void set_timeout(int mode);
extern bool init_wakeup();
extern void set_deep_sleep();
extern int get_timeout_delta();
extern void check_sleep();

#endif
