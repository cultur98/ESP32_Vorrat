#include <Arduino.h>
#include "vorratConfig.h"

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"
#include "wifiman.h"
#include "hch_stuff.h"
#include "deep_sleep_helper.h"

#include <WebConfig.h>

configStruct vorrat_config;

String params;

void init_params_ger()
{
  TRACE1();
  params.clear();
  params = F("["
  "{"
  "'name':'key',"
  "'label':'Vorrat ID',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  "{"
  "'name':'loc',"
  "'label':'Lagerorte',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  #ifndef VORRAT_APP
  "{"
  "'name':'googleID',"
  "'label':'Google Sheet ID',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  "{"
  "'name':'googleAPIkey',"
  "'label':'Google API Key',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  #endif
  "{"
  "'name':'language',"
  "'label':'Sprache',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'Deutsch'},"
  "{'v':'0','l':'Englisch'}],"
  "'default':'1'"
  "},"
  "{"
  "'name':'orientation',"
  "'label':'Bildschirmausrichtung',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'portrait','l':'hochkant'},"
  "{'v':'landscape','l':'quer'}],"
#if defined LILYGO_47 || defined LILYGO_29
  "'default':'landscape'"
#else
  "'default':'portrait'"
#endif
  "},"
  "{"
  "'name':'delta_days',"
  "'label':'MHD Warnzeit (Tage)',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'min': 0,"
  "'default':'4'"
  "},");

  params += F(
  "{"
  "'name':'wakeup_time_0',"
  "'label':'Weckzeit 1 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
  "'default':''"
  "},");

  params += F(
  "{"
  "'name':'wakeup_time_1',"
  "'label':'Weckzeit 2 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
 "'default':''"
  "},");

  params += F(
  "{"
  "'name':'wakeup_time_2',"
  "'label':'Weckzeit 3 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
  "'default':''"
  "},");

  params += F("{"
  "'name':'time_zone_offset',"
  "'label':'Zeitzone',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'MEZ'},"
  "{'v':'2','l':'MESZ'}],"
  "'default':'2'"
  "},");

#ifdef LILYGO_47
  params += F("{"
  "'name':'font_size',"
  "'label':'Schriftgröße',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'14','l':'14'},"
  "{'v':'16','l':'16'},"
  "{'v':'18','l':'18'}],"
  "'default':'16'"
  "},");

  params += F("{"
  "'name':'last_font_size',"
  "'label':'Größe letzte Zeile',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'14','l':'14'},"
  "{'v':'16','l':'16'},"
  "{'v':'18','l':'18'}],"
  "'default':'16'"
  "},");
  params += F("{"
  "'name':'normal_font_style',"
  "'label':'Schrifttyp Standard',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'dünn'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'1'"
  "},");

  params += F("{"
  "'name':'expired_font_style',"
  "'label':'Schrifttyp abgelaufen',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'dünn'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'3'"
  "},");

  params += F("{"
  "'name':'lastline_font_style',"
  "'label':'Schrifttyp letzte Zeile',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'dünn'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'line_space',"
  "'label':'Zeilenabstand',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'17'"
  "},"
  "{"
  "'name':'left_offset',"
  "'label':'Abstand Links',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'10'"
  "},"
  "{"
  "'name':'top_offset',"
  "'label':'Abstand Oben',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'15'"
  "},"
  "{"
  "'name':'bottom_offset',"
  "'label':'Abstand Unten',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'7'"
  "},"
  "{"
  "'name':'date_offset',"
  "'label':'Abstand Datum',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'370'"
  "},"
  "{"
  "'name':'date_gap',"
  "'label':'Lücke Datum',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'20'"
  "},"
  "{"
  "'name':'time_offset',"
  "'label':'Abstand Zeit',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'430'"
  "},"
  "{"
  "'name':'batt_offset',"
  "'label':'Abstand Batteriezustand',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'730'"
  "},"
  "{"
  "'name':'version_offset',"
  "'label':'Abstand Versionsnummer',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'870'"
  "},"); 
  params += F("{"
  "'name':'columns',"
  "'label':'Zahl der Spalten',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'1'},"
  "{'v':'2','l':'2'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'column_offset',"
  "'label':'Abstand Spalten',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'505'"
  "}"
  "]");
#else
  params += F("{"
  "'name':'font_size',"
  "'label':'Schriftgröße',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'8','l':'8'},"
  "{'v':'10','l':'10'},"
  "{'v':'12','l':'12'}],"
  #ifdef LILYGO_29
  "'default':'8'"
  #else
  "'default':'10'"
  #endif
  "},");

  params += F("{"
  "'name':'last_font_size',"
  "'label':'Größe letzte Zeile',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'8','l':'8'},"
  "{'v':'10','l':'10'},"
  "{'v':'12','l':'12'}],"
  "'default':'8'"
  "},");
  params += F("{"
  "'name':'normal_font_style',"
  "'label':'Schrifttyp Standard',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'2'"
  "},");

  params += F("{"
  "'name':'expired_font_style',"
  "'label':'Schrifttyp abgelaufen',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'3'"
  "},");

  params += F("{"
  "'name':'lastline_font_style',"
  "'label':'Schrifttyp letzte Zeile',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'fett'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'line_space',"
  "'label':'Zeilenabstand',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "},"
  "{"
  "'name':'left_offset',"
  "'label':'Abstand Links',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'4'"
#else
  "'default':'0'"
#endif
  "},"
  "{"
  "'name':'top_offset',"
  "'label':'Abstand Oben',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "},"
  "{"
  "'name':'bottom_offset',"
  "'label':'Abstand Unten',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "},"
  "{"
  "'name':'date_offset',"
  "'label':'Abstand Datum',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'264'"
#else
  "'default':'137'"
#endif
  "},"
  "{"
  "'name':'date_gap',"
  "'label':'Lücke Datum',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'4'"
#else
  "'default':'2'"
#endif
  "},"
  "{"
  "'name':'time_offset',"
  "'label':'Abstand Zeit',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'100'"
#else
  "'default':'55'"
#endif
  "},"
  "{"
  "'name':'version_offset',"
  "'label':'Abstand Versionsnummer',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'230'"
#else
  "'default':'145'"
#endif
  "},"); 
  params += F("{"
  "'name':'columns',"
  "'label':'Zahl der Spalten',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'1'},"
  "{'v':'2','l':'2'}],"
  "'default':'1'"
  "},");
  params += F("{"
  "'name':'column_offset',"
  "'label':'Lücke Spalten',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "}"
  "]");
#endif

}

void init_params_eng()
{
  TRACE1();
  params.clear();
  params = F("["
  "{"
  "'name':'key',"
  "'label':'Vorrat Key',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  "{"
  "'name':'loc',"
  "'label':'Storage Location',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  #ifndef VORRAT_APP
  "{"
  "'name':'googleID',"
  "'label':'Google Sheet ID',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  "{"
  "'name':'googleAPIkey',"
  "'label':'Google API Key',"
  "'type':");
  params += String(INPUTTEXT);
  params += F(","
  "'default':''"
  "},"
  #endif
  "{"
  "'name':'language',"
  "'label':'Language',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'German'},"
  "{'v':'0','l':'English'}],"
  "'default':'1'"
  "},"
  "{"
  "'name':'orientation',"
  "'label':'Screen Orientation',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'portrait','l':'portrait'},"
  "{'v':'landscape','l':'landscape'}],"
#if defined LILYGO_47 || defined LILYGO_29
  "'default':'landscape'"
#else
  "'default':'portrait'"
#endif
"},"
  "{"
  "'name':'delta_days',"
  "'label':'BBD Warning (days)',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'min': 0,"
  "'default':'4'"
  "},");

  params += F("{"
  "'name':'wakeup_time_0',"
  "'label':'Wakeup 1 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
  "'default':''"
  "},");

  params += F("{"
  "'name':'wakeup_time_1',"
  "'label':'Wakeup 2 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
  "'default':''"
  "},");

  params += F("{"
  "'name':'wakeup_time_2',"
  "'label':'Wakeup 3 [hh:mm]',"
  "'type':");
  params += String(INPUTTIME);
  params += F(","
  "'default':''"
  "},");

  params += F("{"
  "'name':'time_zone_offset',"
  "'label':'Time Zone Offset',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'MET'},"
  "{'v':'2','l':'MEST'}],"
  "'default':'2'"
  "},");

#ifdef LILYGO_47
  params += F("{"
  "'name':'font_size',"
  "'label':'Font Size',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'14','l':'14'},"
  "{'v':'16','l':'16'},"
  "{'v':'18','l':'18'}],"
  "'default':'16'"
  "},");

  params += F("{"
  "'name':'last_font_size',"
  "'label':'Last Line Font Size',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'14','l':'14'},"
  "{'v':'16','l':'16'},"
  "{'v':'18','l':'18'}],"
  "'default':'16'"
  "},");
  params += F("{"
  "'name':'normal_font_style',"
  "'label':'Normal Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'light'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'1'"
  "},");

  params += F("{"
  "'name':'expired_font_style',"
  "'label':'Expired Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'light'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'3'"
  "},");

  params += F("{"
  "'name':'lastline_font_style',"
  "'label':'Last Line Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'light'},"
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'line_space',"
  "'label':'Line Spacing',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'17'"
  "},"
  "{"
  "'name':'left_offset',"
  "'label':'Left Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'10'"
  "},"
  "{"
  "'name':'top_offset',"
  "'label':'Top Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'15'"
  "},"
  "{"
  "'name':'bottom_offset',"
  "'label':'Bottom Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'7'"
  "},"
  "{"
  "'name':'date_offset',"
  "'label':'Date Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'370'"
  "},"
  "{"
  "'name':'date_gap',"
  "'label':'Date Gap',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'20'"
  "},"
  "{"
  "'name':'time_offset',"
  "'label':'Time Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'430'"
  "},"
  "{"
  "'name':'batt_offset',"
  "'label':'Battery Status Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'730'"
  "},"
  "{"
  "'name':'version_offset',"
  "'label':'Version Number Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'870'"
  "},"); 
  params += F("{"
  "'name':'columns',"
  "'label':'Number of Columns',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'1'},"
  "{'v':'2','l':'2'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'column_offset',"
  "'label':'Column Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'505'"
  "}"
  "]");
#else
  params += F("{"
  "'name':'font_size',"
  "'label':'Font Size',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'8','l':'8'},"
  "{'v':'10','l':'10'},"
  "{'v':'12','l':'12'}],"
  #ifdef LILYGO_29
  "'default':'8'"
  #else
  "'default':'10'"
  #endif
  "},");

  params += F("{"
  "'name':'last_font_size',"
  "'label':'Last Line Font Size',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'8','l':'8'},"
  "{'v':'10','l':'10'},"
  "{'v':'12','l':'12'}],"
  "'default':'8'"
  "},");
  params += F("{"
  "'name':'normal_font_style',"
  "'label':'Normal Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'2'"
  "},");

  params += F("{"
  "'name':'expired_font_style',"
  "'label':'Expired Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'3'"
  "},");

  params += F("{"
  "'name':'lastline_font_style',"
  "'label':'Last Line Font Style',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'2','l':'normal'},"
  "{'v':'3','l':'bold'}],"
  "'default':'2'"
  "},");
  params += F("{"
  "'name':'line_space',"
  "'label':'Line Spacing',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'4'"
  "},"
  "{"
  "'name':'left_offset',"
  "'label':'Left Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'4'"
#else
  "'default':'0'"
#endif
  "},"
  "{"
  "'name':'top_offset',"
  "'label':'Top Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'2'"
  "},"
  "{"
  "'name':'bottom_offset',"
  "'label':'Bottom Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "},"
  "{"
  "'name':'date_offset',"
  "'label':'Date Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'264'"
#else
  "'default':'137'"
#endif
  "},"
  "{"
  "'name':'date_gap',"
  "'label':'Date Gap',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'4'"
#else
  "'default':'2'"
#endif
  "},"
  "{"
  "'name':'time_offset',"
  "'label':'Time Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'100'"
#else
  "'default':'55'"
#endif
  "},"
  "{"
  "'name':'version_offset',"
  "'label':'Version Number Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
#ifdef LILYGO_29
  "'default':'230'"
#else
  "'default':'145'"
#endif
  "},"); 
  params += F("{"
  "'name':'columns',"
  "'label':'Number of Columns',"
  "'type':");
  params += String(INPUTSELECT);
  params += F(","
  "'options':["
  "{'v':'1','l':'1'},"
  "{'v':'2','l':'2'}],"
  "'default':'1'"
  "},");
  params += F("{"
  "'name':'column_offset',"
  "'label':'Column Offset',"
  "'type':");
  params += String(INPUTNUMBER);
  params += F(","
  "'default':'0'"
  "}"
  "]");
#endif

}

void init_params()
{ 
  DUMP1(language);
  switch (language)
  {
  case _GER_:
    init_params_ger();
    break;
  
  default:
    init_params_eng();
    break;
  }
}

WebConfig conf;

void updateConfig()
{
  TRACE1();
  vorrat_config.key = conf.getString("key");
  vorrat_config.loc = conf.getString("loc");
  vorrat_config.googleID = conf.getString("googleID");
  vorrat_config.googleAPIkey = conf.getString("googleAPIkey");
  vorrat_config.orient =  conf.getString("orientation");
  vorrat_config.delta_days =  conf.getInt("delta_days");
  vorrat_config.wakeup_time[0] = conf.getString("wakeup_time_0");
  vorrat_config.wakeup_time[1] = conf.getString("wakeup_time_1");
  vorrat_config.wakeup_time[2] = conf.getString("wakeup_time_2");
  vorrat_config.time_zone_offset = conf.getInt("time_zone_offset");
  vorrat_config.font_size =  conf.getInt("font_size");
  vorrat_config.last_font_size =  conf.getInt("last_font_size");
  vorrat_config.normal_font_style =  conf.getInt("normal_font_style");
  vorrat_config.expired_font_style =  conf.getInt("expired_font_style");
  vorrat_config.lastline_font_style =  conf.getInt("lastline_font_style");
  vorrat_config.line_space =  conf.getInt("line_space");
  vorrat_config.left_offset =  conf.getInt("left_offset");
  vorrat_config.top_offset =  conf.getInt("top_offset");
  vorrat_config.bottom_offset =  conf.getInt("bottom_offset");
  vorrat_config.date_offset =  conf.getInt("date_offset");
  vorrat_config.date_gap =  conf.getInt("date_gap");
  vorrat_config.time_offset =  conf.getInt("time_offset");
  vorrat_config.columns =  conf.getInt("columns");
  vorrat_config.column_offset =  conf.getInt("column_offset");
  vorrat_config.batt_offset =  conf.getInt("batt_offset");
  vorrat_config.version_offset =  conf.getInt("version_offset");
  language = conf.getInt("language");
  hch_set_lang(language);
  DUMP1(language);
}

void handleConfig() {
  TRACE1();
  DUMP1(language);
  conf.handleFormRequest(&server);
  set_timeout(LONG_TIMEOUT);
  if (server.hasArg("SAVE")) {
    uint8_t cnt = conf.getCount();
    Serial.println(F("*********** Konfiguration ************"));
    for (uint8_t i = 0; i<cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(F(" = "));
      Serial.println(conf.values[i]);
    }
    Serial.println(F("**************************************"));
    updateConfig();
    if (conf.getBool("switch"))
     Serial.println(F("SWITCH"));
  }
}

bool initConfig() {
  TRACE1();
  init_params();
  conf.setDescription(params);
  bool ret = conf.readConfig();
  if(ret == true)
    updateConfig();
  return(ret);
}
