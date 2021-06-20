#include "ota.h"
#include "Config.h"
#include "deep_sleep_helper.h"
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HttpsOTAUpdate.h>

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"

#ifdef VORRAT_APP
#define OTA_URL "https://vorrat.sebag.de/esp32/"
#else
#define OTA_URL "https://github.com/cultur98/ESP32_Vorrat/tree/main/bin.dev"
#endif

vorratVersion theVersion;

bool ota_update_started;
const char* fwUrlBase = OTA_URL;

String getVersion()
{
  TRACE1();
  String result;
  result += "esp32_vorrat_";

#ifdef LILYGO_21
  result += "lilygo_21";
#endif
#ifdef LILYGO_27
  result += "lilygo_27";
#endif
#ifdef LILYGO_29
  result += "lilygo_29";
#endif
#ifdef LILYGO_47
  result += "lilygo_47";
#endif

#ifndef VORRAT_APP
  result += "dev";
#endif
  DUMP1(result);
  return String( result );
}

bool checkVersion() {
  TRACE1();
  bool fw_update = false;
  WiFiClientSecure client;
  client.setInsecure();
  DUMP1(fwUrlBase);
  String espversion = getVersion();
  String fwURL = String( fwUrlBase );
  fwURL.concat( espversion );
  String fwVersionURL = fwURL;
  fwVersionURL.concat( ".version" );

  Serial.println( "Checking for firmware updates." );
  Serial.print( "Version: " );
  Serial.println( espversion );
  Serial.print( "Firmware version URL: " );
  Serial.println( fwVersionURL );

  HTTPClient httpClient;
  httpClient.begin( fwVersionURL );
  int httpCode = httpClient.GET();
  if( httpCode == 200 ) {
    String newFWVersion = httpClient.getString();
    theVersion.cur_major = FW_VERSION_MAJ;
    theVersion.cur_minor = FW_VERSION_MIN;
    Serial.printf( "Current firmware version: %d.%d\n", theVersion.cur_major, theVersion.cur_minor);
    const char *newVersion = newFWVersion.c_str();
    int ret = sscanf(newVersion, "%d.%d", &theVersion.new_major, &theVersion.new_minor);
    Serial.printf( "Available firmware version: : %d.%d (%d)\n", theVersion.new_major, theVersion.new_minor, ret);
    if(theVersion.new_major > theVersion.cur_major)
      fw_update = true;

    if(theVersion.new_major == theVersion.cur_major)
      if(theVersion.new_minor > theVersion.cur_minor)
        fw_update = true;

    if( (theVersion.new_major == 0) & (theVersion.new_minor == 0))
      fw_update = true;
  }
  else {
    Serial.print( "Firmware version check failed, got HTTP response code " );
    Serial.println( httpCode );
    theVersion.new_major = -1;
    theVersion.new_minor = -1;
  }
  httpClient.end();
  return(fw_update);
}

void HttpEvent(HttpEvent_t *event)
{
  TRACE1();
  switch(event->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("Http Event Error");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("Http Event On Connected");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("Http Event Header Sent");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("Http Event On Finish");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("Http Event Disconnected");
      break;
  }
}

bool checkForUpdates() {
  TRACE1();
  WiFiClientSecure client;
  client.setInsecure();
  String espversion = getVersion();
  String fwURL = String( fwUrlBase );
  fwURL.concat( espversion );
  String fwVersionURL = fwURL;
  fwVersionURL.concat( ".version" );

  Serial.println( "Checking for firmware updates." );
  Serial.print( "Version: " );
  Serial.println( espversion );
  Serial.print( "Firmware version URL: " );
  Serial.println( fwVersionURL );

  bool fw_update = false;
  if(theVersion.new_major > theVersion.cur_major)
    fw_update = true;

  if(theVersion.new_major == theVersion.cur_major)
    if(theVersion.new_minor > theVersion.cur_minor)
      fw_update = true;

  if( (theVersion.new_major == 0) & (theVersion.new_minor == 0))
    fw_update = true;

  if( fw_update == true ) {
    Serial.println( "Preparing to update" );
    set_timeout(LONG_TIMEOUT);
    String fwImageURL = fwURL;
    fwImageURL.concat( ".bin" );
    Serial.printf("update firmware %s\n", fwImageURL.c_str());
    ota_update_started = true;
    t_httpUpdate_return ret = httpUpdate.update( client, fwImageURL );
    switch(ret) {
      case HTTP_UPDATE_OK:
        Serial.printf("HTTP_UPDATE_OK to new firmware\n");
        break;

    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
    }
  }
  return(fw_update);
}

/*
bool checkForUpdates() {
}

bool checkVersion() {
  theVersion.cur_major = FW_VERSION_MAJ;
  theVersion.cur_minor = FW_VERSION_MIN;
  theVersion.new_major = FW_VERSION_MAJ;
  theVersion.new_minor = FW_VERSION_MIN;
  return(false);
}
*/
