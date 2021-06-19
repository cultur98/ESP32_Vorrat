#include "ePaperBWR.h"
#include "lilygo.h"

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"
#include "Logger.h"
#include "wifiman.h"
#include "vorratConfig.h"
#include "batty.h"

ePaperBWR::ePaperBWR()
{
}

void ePaperBWR::loadSettings()
{
  TRACE1();

  if(vorrat_config.orient.equals("portrait"))
    orientation = PORTRAIT_MODE;
  else
    orientation = LANDSCAPE_MODE;
  DUMP1(orientation);
  Serial.println(vorrat_config.orient);

  width = s_width;
  height = s_height;
  if(orientation == LANDSCAPE_MODE)
  {
#ifndef LILYGO_47
    int t_width = s_width;
    width = s_height;
    height = t_width;
#endif
  }
  EPD_orient(orientation);
  delta_days = vorrat_config.delta_days;
  last_size = vorrat_config.last_font_size;
  font_size = vorrat_config.font_size;
  line_space = vorrat_config.line_space;
  left_space = vorrat_config.left_offset;
  top_space = vorrat_config.top_offset;
  bottom_space = vorrat_config.bottom_offset;
  x_date = vorrat_config.date_offset;
  gap_space = vorrat_config.date_gap;
  x_time = vorrat_config.time_offset;
  x_version = vorrat_config.version_offset;
  x_batt = vorrat_config.batt_offset;
  columns = vorrat_config.columns;
  column_space = vorrat_config.column_offset;

 #if defined LILYGO_21 || defined LILYGO_27 || defined LILYGO_29
  switch (font_size)
  {
  case 8:
    break;
  
  case 12:
    break;

  default:
    font_size = 10;
    break;
  }

  switch (last_size)
  {
  case 8:
    break;
  
  case 12:
    break;

  default:
    last_size = 10;
    break;
  }
#endif

  int last_line_offset = font_size - last_size;
  items_per_screen = (height - last_size-top_space - bottom_space + last_line_offset)/(font_size+line_space);
  DUMP1(width);
  DUMP1(height);
  DUMP2(items_per_screen);
  DUMP2(delta_days);
  DUMP2(last_size);
  DUMP2(font_size);
  DUMP2(line_space);
  DUMP2(x_date);
  DUMP2(gap_space);
  DUMP2(x_time);
  DUMP2(left_space);
  DUMP2(top_space);
  DUMP2(bottom_space);
  DUMP2(columns);
  DUMP2(column_space);
}

void ePaperBWR::init()
{
  TRACE1();
  EPD_init(s_width, s_height);
  DUMP1(s_width);
  DUMP1(s_height);
  delta_days = 0;
  last_size = 10;
  font_size = 10;
  line_space = 2;
  x_date = 137;
  gap_space = 5;
  x_time = 50;
  left_space = top_space = bottom_space = 0;
}

void ePaperBWR::print_lager_list(char *update, int today, int page, bool new_fw)
{
  TRACE1();
  DUMP1(page);
  DUMP1(width);
  DUMP1(height);
  EPD_enter();
  loadSettings();
  lager_list.sort(comp_list);
//  lager_list.resize(5);
  std::list<lager_item>::iterator it = lager_list.begin();
  DUMP1(today);
  int columns_per_screen = items_per_screen;
  items_per_screen = items_per_screen * columns;
  DUMP1(items_per_screen);
  int line_num=0;
  int item_num=0;
  int screens = lager_list.size()/(items_per_screen+1);
  DUMP1(screens);
  DUMP1(lager_list.size());
  if(screens == 0)
    page = 0;
  else
    page = page % (screens+1);
  DUMP1(page);
  int page_offset = page * items_per_screen;
  int ypos = font_size + top_space;
  int xpos = left_space;
  int x_rec = x_date - gap_space;
  int rec_width = width - x_rec;
  while(it != lager_list.end())
  {
    if((item_num < (items_per_screen + page_offset)) && (item_num >= page_offset))
    {
      int style;
      line_num++;
      int delta_time = (int)it->unix_time - today;
      if(delta_time < delta_days)
        style = vorrat_config.expired_font_style;
      else
        style = vorrat_config.normal_font_style;
      DUMP2(item_num);
      DUMP2(columns_per_screen);
      if(((item_num-1) % columns_per_screen) == (columns_per_screen-1) && (line_num != 1))
      {
        ypos = font_size + top_space;
        if(columns > 1)
          xpos = column_space;
        EPD_draw_rect(x_rec, 0, rec_width, height);

        DUMP2(xpos);        
        DUMP2(ypos);        
      }
      EPD_print_text(font_size, (char*)it->name.c_str(), xpos, ypos, style);
      ypos = ypos + font_size + line_space;
    }
    it++;
    item_num++;
  }
  if(columns == 2)
  {
    x_rec = column_space + x_date - gap_space;
    rec_width = width - x_rec;
    EPD_draw_rect(x_rec, 0, rec_width, height);
  }
  else
  {
    EPD_draw_rect(x_rec, 0, rec_width, height);
  }
  it = lager_list.begin();
  item_num = 0;
  ypos = font_size + top_space;
  xpos = x_date;
  line_num = 0;
  while(it != lager_list.end())
  {
    if((item_num < (items_per_screen + page_offset)) && (item_num >= page_offset))
    {
      int style;
      line_num++;
      int delta_time = (int)it->unix_time - today;
      if(delta_time < delta_days)
        style = vorrat_config.expired_font_style;
      else
        style = vorrat_config.normal_font_style;

      if(((item_num-1) % columns_per_screen) == (columns_per_screen-1) && (line_num != 1))
      {
        ypos = font_size + top_space;
        if(columns > 1)
          xpos = column_space + x_date;
      }
      EPD_print_text(font_size, (char*)it->date.c_str(), xpos, ypos, style);
      ypos = ypos + font_size + line_space;
    }
    it++;
    item_num++;
  }

  char page_string[10];
  if (screens > 0 ) {
    if (page < screens) 
      sprintf(page_string, "[%d/%d]", page+1, screens+1);
    else
      sprintf(page_string, "[%d]",page+1);   
  }
  else
    sprintf(page_string, "[1]");   


  ypos = height - bottom_space -1;
  char last_text[80];
  DUMP1(batt_voltage);
  if(new_fw == true)
    snprintf(last_text, 80, "%s U", update);
  else
    snprintf(last_text, 80, "%s", update);
    
  EPD_print_text(last_size, last_text, x_time, ypos, vorrat_config.lastline_font_style);
  snprintf(last_text, 80, "v%d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  EPD_print_text(last_size, last_text, x_version, ypos, vorrat_config.lastline_font_style);


  if(x_batt > 0)
  {
//    snprintf(last_text, 80, "%.2f %d%%", batt_voltage, batt_level);
    snprintf(last_text, 80, "%d%%", batt_level);
    DUMP1(batt_voltage);
    DUMP1(batt_level);
    EPD_print_text(last_size, last_text, x_batt, ypos, vorrat_config.lastline_font_style);
  }
  EPD_print_text(last_size, page_string, left_space, ypos, vorrat_config.lastline_font_style);
  EPD_leave();
}

#ifdef LILYGO_47
#define Y_OFFSET 60
#define X_START 50
#define Y_START 80
#else
#define Y_OFFSET 25
#define X_START 10
#define Y_START 25
#endif

void ePaperBWR::ap_mode(const char* mess)
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  if(language == _GER_)
    sprintf(the_string, "AP Modus | SW v%d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  else
    sprintf(the_string, "AP MODE | SW v%d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  EPD_print_text(12, the_string, X_START, Y_START, false);
 
  sprintf(the_string, "SSID  %s", ssid.c_str());
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);

  sprintf(the_string, "http://%s", "192.168.100.1");
  EPD_print_text(12, the_string, X_START, Y_START+2*Y_OFFSET, false);

  sprintf(the_string, "PW    %s", AP_PASSWORD);
  EPD_print_text(12, the_string, X_START, Y_START+3*Y_OFFSET, false);

  sprintf(the_string, "%s", mess);
  EPD_print_text(12, the_string, X_START, Y_START+4*Y_OFFSET, false);
  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}

void ePaperBWR::show_config()
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  sprintf(the_string, "%s   SW %d.%d", WiFi.getHostname(), FW_VERSION_MAJ, FW_VERSION_MIN);
  EPD_print_text(12, the_string, X_START, Y_START, false);
 
  sprintf(the_string, "SSID %s", WiFi.SSID().c_str());
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);

  sprintf(the_string, "http://%s", WiFi.localIP().toString().c_str());
  EPD_print_text(12, the_string, X_START, Y_START+2*Y_OFFSET, false);

  sprintf(the_string, "WiFi RSSI %d", WiFi.RSSI());
  EPD_print_text(12, the_string, X_START, Y_START+3*Y_OFFSET, false);
  
  if(batt_voltage > 2.0f)
  {
    sprintf(the_string, "Batterie  %.2fV  (%d%%)", batt_voltage, batt_level);
    EPD_print_text(12, the_string, X_START, Y_START+4*Y_OFFSET, false);
  }
  else
  {
    sprintf(the_string, "No Battery");
    EPD_print_text(12, the_string, X_START, Y_START+4*Y_OFFSET, false);
  }
  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}

void ePaperBWR::show_ota_update(int new_major, int new_minor)
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  if(language == _GER_)
    sprintf(the_string, "Online Update läuft");
  else
    sprintf(the_string, "Online Update in progress");
  EPD_print_text(12, the_string, X_START, Y_START, false);
 
  if(language == _GER_)
    sprintf(the_string, "von Version %d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  else
    sprintf(the_string, "From version %d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);

  if(language == _GER_)
    sprintf(the_string, "auf Version %d.%d", new_major, new_minor);
  else
    sprintf(the_string, "to version %d.%d", new_major, new_minor);
  EPD_print_text(12, the_string, X_START, Y_START+2*Y_OFFSET, false);

  if(language == _GER_)
    sprintf(the_string, "Dies kann einige Minuten dauern ..");
  else
    sprintf(the_string, "This can take some minutes ..");
  EPD_print_text(12, the_string, X_START, Y_START+3*Y_OFFSET, false);
  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}

void ePaperBWR::show_firmware()
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  if(language == _GER_)
    sprintf(the_string, "Firmware ist aktuell");  
  else
    sprintf(the_string, "Firmware is up to date");
  EPD_print_text(12, the_string, X_START, Y_START, false);
 
  sprintf(the_string, "Version %d.%d", FW_VERSION_MAJ, FW_VERSION_MIN);
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);

  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}

void ePaperBWR::firmware_change(int new_fw_maj, int new_fw_min)
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  if(language == _GER_)
    sprintf(the_string, "Software aktualisiert auf");
  else
    sprintf(the_string, "Software update to");
  EPD_print_text(12, the_string, X_START, Y_START, false);

  if(language == _GER_)
    sprintf(the_string, "Version %d.%d", new_fw_maj, new_fw_min);
  else
    sprintf(the_string, "version %d.%d", new_fw_maj, new_fw_min);
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);
  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}

void ePaperBWR::default_mode()
{
  TRACE1();

  EPD_enter();
  EPD_orient(LANDSCAPE_MODE);

  char *the_string = (char*)malloc(MED_STRING_LEN);
  if(language == _GER_)
    sprintf(the_string, "Gerät muss konfigurtiert werden!");
  else
    sprintf(the_string, "Device has to be configured!");
  EPD_print_text(12, the_string, X_START, Y_START, false);

  sprintf(the_string, "SSID %s", WiFi.SSID().c_str());
  EPD_print_text(12, the_string, X_START, Y_START+Y_OFFSET, false);

  sprintf(the_string, "http://%s", WiFi.localIP().toString().c_str());
  EPD_print_text(12, the_string, X_START, Y_START+2*Y_OFFSET, false);

  if(language == _GER_)
  {
    sprintf(the_string, "Start: Knopf 1-> Online Update");
    EPD_print_text(12, the_string, X_START, Y_START+3*Y_OFFSET, false);
    sprintf(the_string, "  Knopf 2 -> zeige Konfiguration");
    EPD_print_text(12, the_string, X_START, Y_START+4*Y_OFFSET, false);
  }
  else
  {
    sprintf(the_string, "Start: Knopf 1-> Online Update");
    EPD_print_text(12, the_string, X_START, Y_START+3*Y_OFFSET, false);
    sprintf(the_string, "  Knopf 2 -> zeige Konfiguration");
    EPD_print_text(12, the_string, X_START, Y_START+4*Y_OFFSET, false);
  }
  EPD_draw_logo();
  EPD_leave();
  free(the_string);
}
