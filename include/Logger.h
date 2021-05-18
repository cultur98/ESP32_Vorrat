#ifndef _LOGGER_H
#define _LOGGER_H
#include <Arduino.h>
#include <WiFiClient.h>
#include "Config.h"
#include "wifiman.h"

#include <FS.h>     
#include <list>

#define MAX_UINT32 4294967295
#define MAX_LINES 25
#define LAGER_FILE "lagerlsit.txt"

typedef struct lager_item
{
  int num;
  String name;
  String date;
  String storeage;
  uint32_t unix_time;
} lager_item;

extern std::list<lager_item> lager_list;
extern std::list<String> lager_orte;
extern bool comp_list(const lager_item& item1, const lager_item& item2);

class Logger {
public:
  Logger();
  bool init();
  bool get_lager_list_server(const char *key, const char *ort);
  bool get_google_list(const char *sheet_id, const char *api_key);
  void split_lager_line(String, int);
  void split_lager_list(String);
  void split_lager_orte();
  bool dumpFile();
  void dumpLagerList();

private:
  int name_col = -1;
  int storage_col = -1;
  int date_col = -1;
  int item_num = 0;
  String formatBytes(size_t bytes); 
  String split(String s, char parser, int index); 
  uint32_t string_to_seconds(const char *timestamp_str);
  int parse_google_list(String payload);
  int get_google_list_part(char* web_adress);
};

#endif
