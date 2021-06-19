#ifndef _WIFI_MAN_H
#define _WIFI_MAN_H

#include "Config.h"
#include <WiFi.h>
#include <WebServer.h>

extern IPAddress APStaticIP;
extern IPAddress stationIP;
extern String ssid;
extern String ssid1;

extern WebServer server;
extern void WiFiManager_init(const char*, bool);
extern void WiFiManager_loop();
extern bool WiFiManager_readConfigFile();
extern bool WiFiManager_loadConfigData();
extern char vorratOrt[];
extern char vorratOri[];
extern char vorratDelta[];
extern char vorratKey[];
extern char googleSheetID[];
extern char googleApiKey[];

#endif
