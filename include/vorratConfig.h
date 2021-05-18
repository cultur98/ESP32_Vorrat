#ifndef _VORRAT_CONFIG_H
#define _VORRAT_CONFIG_H
#include "Config.h"

#define WAKE_UP_TIMES 3
typedef struct _configStruct {
  String key;
  String loc; 
  String googleID;
  String googleAPIkey;
  String language;
  String orient;
  int delta_days;
  String wakeup_time[WAKE_UP_TIMES];
  int time_zone_offset;
  int font_size;
  int last_font_size;
  int normal_font_style;
  int expired_font_style;
  int lastline_font_style;
  int line_space;
  int left_offset;
  int top_offset;
  int bottom_offset;
  int date_offset;
  int date_gap;
  int time_offset;
  int version_offset;
  int batt_offset;
  int columns;
  int column_offset;
} configStruct;

extern void handleConfig();
extern bool initConfig();
extern configStruct vorrat_config;

#endif