#ifndef _E_PAPER_BWR_H
#define _E_PAPER_BWR_H
#include "Config.h"

#define PORTRAIT_MODE 0
#define LANDSCAPE_MODE 1

class ePaperBWR {
public:
  ePaperBWR();
  void init();
  void print_lager_list(char *, int, int, bool);
  void firmware_change(int, int);
  void ap_mode(const char*);
  void show_ota_update(int, int);
  void show_config();
  void show_firmware();
  void default_mode();
  uint8_t orientation;
private:
  int delta_days;
  int font_size;
  int last_size;
  int line_space;
  int gap_space, x_time, x_date;
  int x_version, x_batt;
  int left_space, top_space, bottom_space;
  int width, s_width;
  int height, s_height;
  int columns, column_space;
  int items_per_screen;
  void loadSettings();

};

#endif