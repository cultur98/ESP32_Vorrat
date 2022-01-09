#include "Logger.h"
#include <WiFiClient.h>
#include <list>
#include "hch_stuff.h"
#include "vorratConfig.h"

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"

WiFiClient client;

std::list<lager_item> lager_list;
std::list<String> lager_orte;

Logger::Logger() {
  TRACE1();
}

bool Logger::init() {
  TRACE1();
  return(true);
}

String Logger::split(String s, char parser, int index) {
  TRACE2();
  String rs="";
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;
  while (index >= parserCnt) {
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);
    if (index == parserCnt) {
      if (rToIndex == 0 || rToIndex == -1) return "";
      return s.substring(rFromIndex,rToIndex);
    } else parserCnt++;
  }
  return rs;
}

uint32_t Logger::string_to_seconds(const char *timestamp_str)
{
  DUMP2(timestamp_str);
  struct tm tm;
  time_t seconds;
  int r;

  if (timestamp_str == NULL) {
    Serial.println(F("null argument"));
    return (time_t)-1;
  }
  r = sscanf(timestamp_str, "%d.%d.%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
  if (r != 3) {
    printf("expected %d numbers scanned in %s\n", r, timestamp_str);
    return (MAX_UINT32);
  }
  tm.tm_hour=23;
  tm.tm_min=59;
  tm.tm_sec=59;
  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  tm.tm_isdst = 0;
  seconds = mktime(&tm);
  if (seconds == (time_t)-1) {
    printf("reading time from %s failed\n", timestamp_str);
    return(MAX_UINT32);
  }
  DUMP2(seconds);
  return (uint32_t)seconds;
}

void Logger::split_lager_line(String line_cont, int line_num) {
  lager_item new_item;
  TRACE2();
  if(line_num > 1)
  {
    new_item.num = line_num - 1;
    new_item.name = split(line_cont, '\t', 0).c_str();
    if(new_item.date.isEmpty())
    {
      new_item.date = new_item.storeage;
      new_item.storeage.clear();  
    }
    new_item.date = split(line_cont, '\t', 4).c_str();
    new_item.date.remove(6,100);
    new_item.unix_time = string_to_seconds(split(line_cont, '\t', 4).c_str())/(60*60*24);
    lager_list.push_back(new_item);
  }
}  

void Logger::split_lager_orte() {
  TRACE1();
  char *item_ptr;
  char buffer[MAX_STRING_LEN];
  strcpy(buffer, vorrat_config.loc.c_str());
  char *buf_ptr = buffer;
  int item_num=0;
  while ((item_ptr = strtok_r(buf_ptr, ";", &buf_ptr)) != NULL)
  {  
    item_num++;
    Serial.printf("%d - %s\n", item_num, item_ptr);
    String ort = item_ptr;
    lager_orte.push_back(ort);
  }
}

void Logger::split_lager_list(String payload) {
  TRACE1();
  split_lager_orte();
  lager_item new_item;
  int line_num = 0;
  char *buf_ptr = (char*) payload.c_str();
  char *line_ptr;
  bool found_ort=true;
  while ((line_ptr = strtok_r(buf_ptr, "\n", &buf_ptr)) != NULL)
  { 
    int item_num = 0;
    char *the_line = (char*)malloc(strlen(line_ptr)+1);
    char *the_line_ptr = the_line; 
    char *item_ptr;
    strcpy(the_line, line_ptr);
    line_num++;
    if(line_num == 1)
      continue;
//    Serial.printf("%d - %s\n", line_num, line_ptr);
    while ((item_ptr = strtok_r(the_line_ptr, "\t", &the_line_ptr)) != NULL)
    {  
      item_num++;
//      Serial.printf("%d.%d - %s\n", line_num, item_num, item_ptr);
      switch(item_num)
      {
        case 1:
          new_item.name = item_ptr;
          break;
        case 2:
          new_item.storeage = item_ptr;
          if(lager_orte.size() > 0)
          {
            found_ort=false;
            std::list<String>::iterator it = lager_orte.begin();
            while(it != lager_orte.end())
            {
              if(new_item.storeage.equals(*it))
              {
                found_ort = true; 
                Serial.printf("Ort [%d] [%s] found!\n", item_num, it->c_str());
              }
              it++;
            }
          }
          break;
        case 3:
          new_item.date = item_ptr;
          break;
        default:
          Serial.println(F("Too many items .."));
          break;
      }
    }
    if(found_ort == true)
    {
      new_item.num = line_num;
      new_item.unix_time = string_to_seconds(new_item.date.c_str())/(60*60*24);
      new_item.date.remove(6,100);
      lager_list.push_back(new_item);
      free(the_line);
    }
    else
    {
      Serial.printf("Item %s is not in storage list. Storage is %s\n", new_item.name.c_str(), new_item.storeage.c_str());
    }
  }  
}  

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

bool Logger::get_lager_list_server(const char *key, const char *ort) {
  TRACE1();
  HTTPClient http;
  DUMP1(key);
  DUMP1(ort);
  char web_adress[MAX_STRING_LEN];
  if( (key != NULL) && (strlen(key) > 0))
  {
    sprintf(web_adress, "%s%s", LAGER_WEB_LINK, key);
    DUMP1(web_adress);
  }
  else
  {
    Serial.println(F("No valid key to access Vorratsliste!"));
    return false;
  }
  DUMP2(esp_32_get_free_heap());
  Serial.println(F("Server -> "));
  http.begin(web_adress); 
  int httpCode = http.GET();  //send GET request
  if (httpCode != HTTP_CODE_OK) {
    Serial.print(F("Error on HTTP request: "));
    Serial.println(httpCode);
    http.end();
    return false;
  } else {
    Serial.print(F(" connected!"));
    String payload = http.getString();
    DUMP1(payload);
    split_lager_list(payload);
    payload.remove(0);
  }

    //////
#if ARDUINOTRACE_ENABLE > 1
  dumpLagerList();
#endif
  TRACE1();
  http.end();
  return true;
}

bool comp_list(const lager_item& item1, const lager_item& item2) {
    return (item1.unix_time < item2.unix_time);
}

void Logger::dumpLagerList(){
  TRACE1();
  std::list<lager_item>::iterator it = lager_list.begin();
  Serial.println(F("LAGER LIST DUMP"));
	while(it != lager_list.end()){
    Serial.printf("[%d] [%s][%s][%s] %d\n", it->num, it->name.c_str(), it->date.c_str(), it->storeage.c_str(), it->unix_time);
		it++;
	}
}

bool Logger::dumpFile(){
  return(false);
}

#ifndef VORRAT_APP
#define PRODUKT_HEADER "Produkt"
#define LAGERORT_HEADER "Lagerort"
#define MHD_HEADER "MHD"
#define LIST_OFFSET 25

int Logger::parse_google_list(String payload) {
  DUMP1(payload.length());
  // the following is based on ArduinoJson v6 API
  StaticJsonDocument<4000> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.print(F("deserialization error "));
    Serial.println(err.c_str());
    return -1;
  }

  int i=0;
  int read_items=0;
  if(name_col == -1)
  {
    for(int i=0; i < 100; i++)
    {
      const char* val_string = doc["values"][0][i];
      if(val_string != NULL)
      {
        Serial.printf("[%d] <%s>\n", i, val_string);
        if (strcmp(val_string, PRODUKT_HEADER) == 0)
        {
          name_col = i;
        }  
        if (strcmp(val_string, LAGERORT_HEADER) == 0)
        {
          storage_col = i;
        }  
        if (strcmp(val_string, MHD_HEADER) == 0)
        {
          date_col = i;
        }  
      }
    }
    i=1;
  }

  if( (name_col == -1) || (storage_col == -1) || (date_col == -1) )
  {
    Serial.printf("Table content is not complete\n");
    return(-1);
  }

  for(; i < LIST_OFFSET+1; i++)
  {
    const char* val_str = doc["values"][i][name_col];
    if((val_str != NULL) && (strlen(val_str) > 0))
    {
      lager_item new_item;
      String name = doc["values"][i][name_col];
      String storage = doc["values"][i][storage_col];
      String date = doc["values"][i][date_col];
      if(storage.isEmpty())
      {
        Serial.print(name);
        Serial.println(F(" - no storage defined."));
      }
      new_item.name = name;
      new_item.storeage = storage;
      new_item.date = date;
      new_item.unix_time = string_to_seconds(new_item.date.c_str())/(60*60*24);
      new_item.date.remove(6,100);
      new_item.num = ++item_num;
      read_items++;
      lager_list.push_back(new_item);
    }
  }
  DUMP1(read_items);
  return read_items;
}

bool Logger::get_google_list(const char *sheet_id, const char *api_key) {
  TRACE1();
  HTTPClient http;
  char web_adress[MAX_STRING_LEN];
  if( (sheet_id != NULL) && (api_key != NULL) && (strlen(sheet_id)) && (strlen(api_key) > 0) )
  {
    int new_items=1;
    int i=0;
    while(new_items > 0)
    {
      sprintf(web_adress, "%s%s/values/Vorrat!A%d:%d?key=%s", LAGER_GOOGLE_LINK, sheet_id, i+1, i + LIST_OFFSET, api_key);
      {
        http.begin(web_adress); 
        int httpCode = http.GET();  //send GET request
        if (httpCode != 200) {
          Serial.print(F("Error on HTTP request: "));
          Serial.println(httpCode);
          http.end();
          new_items = -1;
        } else {
          String payload = http.getString();
      //    DUMP1(payload);
          new_items = parse_google_list(payload);
        }
        http.end(); //Free the resources
      }
      i = i + LIST_OFFSET;
      DUMP1(new_items);
      DUMP1(web_adress);
    }
#if ARDUINOTRACE_ENABLE > 1
    dumpLagerList();
#endif
    if(item_num > 0)
    {
      return true;
    }
    else
      return false;
  }
  else
  {
    Serial.println(F("No valid key to access to Google sheet!"));
    return false;
  }
  return false;
}
#else

bool Logger::get_google_list(const char *sheet_id, const char *api_key) {
return(false);
}

#endif
