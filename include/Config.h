#ifndef _CONFIG_H
#define _CONFIG_H
#include <Arduino.h>

#define CLIENT_ID "vorrat"
#define MAX_STRING_LEN 256
#define MED_STRING_LEN 128
#define MIN_STRING_LEN 64

#ifdef LILYGO_47
// LILYGO 4.7"
#define BUTTON_LAST    35
#define BUTTON_NEXT    34
#define BUTTON_WAKE    39
#define BAT_SENSE_PIN  36
#define BAT_GAIN 2.0
#else
// LILYGO 2.x"
#define BUTTON_LAST    37
#define BUTTON_NEXT    39
#define BUTTON_WAKE    38
#define BAT_GAIN 1.86      // should be 2.0 in theory but the AD converter seems to make an error
#define BAT_SENSE_PIN A17
#endif

#define MAX_VOLTAGE 4180
#define MIN_VOLTAGE 3000

// WiFi
#define AP_PASSWORD "vorratslager"
#define NTP_SERVER_NAME "time.nist.gov"

// delay times (all in milliseconds)
#define TICK_TASK_DELAY     5000
#define BUTTON_TASK_DELAY     20
#define NTP_UPDATE_TIME     2000

#define SHORT_TIMEOUT_SEC     30
#define MEDIUM_TIMEOUT_SEC    60
#define LONG_TIMEOUT_SEC     300

#define BUF_SIZE 1024

// settings for accessing storage data 
#define LAGER_WEB_LINK "https://vorrat.net/pl/pl.pl?aktion=getmhd&time=J&code="
#define LAGER_WEB_PORT 443
extern int language;

#ifdef VORRAT_APP
#define OTA_URL "https://vorrat.sebag.de/esp32/"
#else
#define OTA_URL "https://raw.githubusercontent.com/cultur98/ESP32_Vorrat/main/bin.dev/"
#define LAGER_GOOGLE_LINK "https://sheets.googleapis.com/v4/spreadsheets/"
#endif

#define _ENG_ 0
#define _GER_ 1
#endif