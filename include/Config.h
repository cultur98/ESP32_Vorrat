#ifndef _CONFIG_H
#define _CONFIG_H
#include <Arduino.h>

#define CLIENT_ID "vorrat"
#define MAX_STRING_LEN 256
#define MED_STRING_LEN 128
#define MIN_STRING_LEN 64

#define OTA_URL "https://vorrat.sebag.de/esp32/"

#define WAKEUP_BUTTON 39
#ifdef LILYGO_47
#define BUTTON_1      34
#define BUTTON_2      35
#else
#define BUTTON_1      37
#define BUTTON_2      38
#endif

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

extern int language;

#define _ENG_ 0
#define _GER_ 1
#endif
