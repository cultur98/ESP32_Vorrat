#ifndef _WIFI_MAN_H
#define _WIFI_MAN_H

#include "Config.h"
#include <WiFi.h>
#include <WebServer.h>

extern IPAddress APStaticIP;
extern IPAddress stationIP;
extern String ssid;

extern WebServer server;
extern void WiFiManager_init(const char*, bool);
extern void WiFiManager_loop();
extern bool WiFiManager_readConfigFile();
extern bool WiFiManager_loadConfigData();
//extern bool WiFiManager_getIntParam(int num, int &value);
//extern bool WiFiManager_getStringParam(int num, char*value);
extern char vorratOrt[];
extern char vorratOri[];
extern char vorratDelta[];
extern char vorratKey[];
extern char googleSheetID[];
extern char googleApiKey[];

/*
#define NUM_PARAM 18
#define VORRAT_KEY       0
#define VORRAT_ORT       1
#define GOOGLE_SHEET_ID  2
#define GOOGLE_API_KEY   3
#define VORRAT_ORIENT    4
#define VORRAT_DELTA     5
#define VORRAT_WAKE      6
#define VORRAT_FONT_SIZE 7
#define VORRAT_LAST_SIZE 8
#define VORRAT_SPACE     9
#define VORRAT_LEFT     10
#define VORRAT_BOTTOM   11
#define VORRAT_TOP      12
#define VORRAT_DATE     13
#define VORRAT_GAP      14
#define VORRAT_XTIME    15
#define VORRAT_COL      16
#define VORRAT_COL_GAP  17

#define PARAMETER_LEN   80

extern char param_id[NUM_PARAM][PARAMETER_LEN];
extern char param_placeholder[NUM_PARAM][PARAMETER_LEN];
extern char param_value[NUM_PARAM][PARAMETER_LEN];

extern FS* filesystem;
*/
#endif
