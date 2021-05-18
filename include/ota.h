#ifndef _OTA_STUFF_H
#define _OTA_STUFF_H
#include <Arduino.h>

extern bool checkForUpdates();
extern bool checkVersion();

typedef struct vorratVersion
{
  int cur_major;
  int cur_minor;
  int new_major;
  int new_minor;
} vorratVersion;

extern vorratVersion theVersion;
#endif
