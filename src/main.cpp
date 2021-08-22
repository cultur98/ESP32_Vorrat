#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Ticker.h>
#include <Button2.h>

#include "Config.h"
#include "vorratConfig.h"
#include "myTimer.h"
#include "wifiman.h"
#include "Logger.h"
#include "hch_stuff.h"
#include "ePaperBWR.h"
#include "lilygo.h"
#include "ota.h"
#include "deep_sleep_helper.h"
#include "batty.h"

// enable debug messages on the console
#define ARDUINOTRACE_ENABLE 1  
#include "ArduinoTrace.h"

Logger myLogger;
ePaperBWR myEpaper;
Ticker watch_tick;
MyTimer myTimer;
TaskHandle_t TickTask;
int language = _GER_;
Button2 button1 = Button2(BUTTON_LAST);
Button2 button2 = Button2(BUTTON_NEXT);
Batty myBatt;

int the_page = 0;
uint32_t timer_ctr = 0;
uint32_t main_loop_ctr = 0;
int today = 0;
char time_string[MIN_STRING_LEN];
bool has_valid_config = false;
bool new_firmware = false;
bool timer_wakeup  = false;
bool wasConnected = true;

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;

#define UP_TIMEOUT_SEC 30
#define TROUBLE_TIMEOUT_MIN 20

void watch_task() {
  TRACE1();
  timer_ctr++;
  char uptime[80];
  int up_seconds = (TICK_TASK_DELAY/1000)* timer_ctr;
  hch_uptime(up_seconds, uptime);
  bool isConnected = WiFi.isConnected();

  Serial.printf("\nisConn %d | wasConn %d | valid_conf %d | timer_wake %d\n",
    isConnected, wasConnected, has_valid_config, timer_wakeup);

  if(up_seconds > UP_TIMEOUT_SEC)
  {
    if(wasConnected == true && isConnected == false && has_valid_config == true && timer_wakeup == true)
    {
      Serial.printf("TROUBLE detected - trying again in %d minutes\n", TROUBLE_TIMEOUT_MIN);
      uint64_t wakeup_us = TROUBLE_TIMEOUT_MIN * (60 * 1000000);
      esp_sleep_enable_timer_wakeup(wakeup_us);
      set_deep_sleep();
    }

    if(wasConnected == true && isConnected == false)
    {
      Serial.println(F("watch_task() print AP mode"));
      myEpaper.ap_mode("No access to WLAN");
      set_timeout(LONG_TIMEOUT);
    }
    Serial.printf("WIFI was %d | is %d\n", wasConnected, isConnected);
    wasConnected = isConnected;
  }
  uint32_t heap = esp_32_get_free_heap();
  uint32_t psram =  ESP.getPsramSize();
  Serial.printf("NTP date %s\n", time_string);
  Serial.printf("nuptime %s | min_uptime %d | %u boots\n", uptime, UP_TIMEOUT_SEC, hch_get_boot_ctr());
  Serial.printf("main_loop: %u |  timer %u | heap %u | psram %u\n", main_loop_ctr, timer_ctr, heap, psram);
  Serial.printf("sleep in %ds\n", get_timeout_delta()); 
  check_sleep();
}

void show_prev_screen()
{
  set_timeout(MEDIUM_TIMEOUT);
  Serial.println(F("UP   clicked"));
  if(the_page > 0)
    the_page = the_page - 1;
  myEpaper.print_lager_list(time_string, today, the_page, false);
  set_timeout(MEDIUM_TIMEOUT);
}

void show_next_screen()
{
  set_timeout(MEDIUM_TIMEOUT);
  Serial.println(F("DOWN clicked"));
  the_page = the_page + 1;
  myEpaper.print_lager_list(time_string, today, the_page, false);
  set_timeout(MEDIUM_TIMEOUT);
}

void click(Button2& btn) {
  TRACE1();
  if (btn == button1) {
      show_prev_screen();
  }
  if ((btn == button2) || (btn == button2) ){
      show_next_screen();
  }
}

#define WAKE_UP_MINS_DELTA_MIN 30 
void processWakeup()
{
  int hours, mins;
  char time_string[MIN_STRING_LEN];
  int min_wakeup_mins = -1;

  if(today <= 0)
  {
    Serial.printf("Time ist not defined. Cannot set wakeup!\n");
    return;
  }

  for(int i = 0; i < WAKE_UP_TIMES; i++)
  {
    strcpy(time_string, vorrat_config.wakeup_time[i].c_str());
    DUMP1(time_string);
    int ret = sscanf(time_string, "%d:%d", &hours, &mins);
    DUMP1(ret);
    if(ret == 2)
    {
      DUMP2(hours);
      DUMP2(mins);
      if((hours >= 0) && (hours <= 23) && (mins >= 0) && (mins <= 59))
      {
        int wakeup_mins = mins + hours * 60;
        DUMP2(wakeup_mins);
        if (myTimer.getTimeStringShort(time_string, MIN_STRING_LEN) == true)
        {
          sscanf(time_string, "%d:%d", &hours, &mins);
          DUMP2(hours);
          DUMP2(mins);
          if((hours >= 0) && (hours <= 23) && (mins >= 0) && (mins <= 59))
          {
            int the_mins = mins + hours * 60;
            DUMP2(the_mins);
            int wakeup_mins_delta = wakeup_mins - the_mins;
            if(wakeup_mins_delta < 0)
            {
              wakeup_mins_delta = wakeup_mins_delta + 1440;
            }
            if(wakeup_mins_delta < WAKE_UP_MINS_DELTA_MIN)
            {
              wakeup_mins_delta = wakeup_mins_delta + 1440;
            }
            if((wakeup_mins_delta < min_wakeup_mins) || (min_wakeup_mins == -1))
            {
              min_wakeup_mins = wakeup_mins_delta;
              DUMP1(min_wakeup_mins);
            }
            DUMP2(wakeup_mins_delta);
          }
        }  
      }
    }
  }
  if(min_wakeup_mins > 0)
  {
    uint64_t wakeup_us = min_wakeup_mins;
    wakeup_us = wakeup_us * 60 * 1000000;
    esp_sleep_enable_timer_wakeup(wakeup_us);
    Serial.printf("ESP will wake up in [%02d:%02d]\n", min_wakeup_mins / 60, min_wakeup_mins % 60);
  }
}

bool setup_timer()
{
  TRACE1();
  int update_ctr = 0;
  myTimer.init(vorrat_config.time_zone_offset);
  while((myTimer.update() == false) && (update_ctr++ < 5))
  {
    Serial.print(F("Failed to get time via NTP.\n"));
    Serial.printf("Trying again in %dms.\n", NTP_UPDATE_TIME);
    delay(NTP_UPDATE_TIME);
  }
  myTimer.getTimeString(time_string, MIN_STRING_LEN);
  today = myTimer.tick() / (60*60*24);
  return(true);
}

bool process_list(bool new_fw)
{
  TRACE1();
  setup_timer();
  myLogger.init();
  if(vorrat_config.key.length() > 0)
  {
    if(myLogger.get_lager_list_server(vorrat_config.key.c_str(), vorrat_config.loc.c_str()) == true)
    {
      myEpaper.print_lager_list(time_string, today, the_page, new_fw);
      set_timeout(SHORT_TIMEOUT);
      processWakeup();
      return(true);
    }
  }
  else if((vorrat_config.googleID.length() > 0)  && (vorrat_config.googleAPIkey.length() > 0))
  {
    if(myLogger.get_google_list(vorrat_config.googleID.c_str(), vorrat_config.googleAPIkey.c_str()) == true)
    {
      myEpaper.print_lager_list(time_string, today, the_page, new_fw);
      set_timeout(SHORT_TIMEOUT);
      processWakeup();
      return(true);
    }
  }
  processWakeup();
  return(false);
}

bool setup_button_click()
{
  TRACE1();
  button1.setClickHandler(click);
  button2.setClickHandler(click);
  button1.setLongClickHandler(click);
  button2.setLongClickHandler(click);
  return(true);
}

void setup() {
  TRACE1();
  bool show_config = false;
  bool ota_update = false;
  bool force_ap_mode = false;
  pinMode(BUTTON_WAKE, INPUT_PULLUP);
  pinMode(BUTTON_LAST, INPUT_PULLUP);
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  ota_update = hch_init((char*)CLIENT_ID, 115200);
  delay(100);
  language = hch_get_lang();
  bootCount++;
  Serial.println("Boot number: " + String(bootCount));

  // init wakeup and deep sleep behavior
  timer_wakeup = init_wakeup();
  set_timeout(LONG_TIMEOUT);

  Serial.printf("CLIENT_ID %s\n", CLIENT_ID);
  watch_tick.attach((float)(TICK_TASK_DELAY / 1000), watch_task);

  Serial.printf("Button state [%d|%d|%d]\n", 
    digitalRead(BUTTON_WAKE), digitalRead(BUTTON_LAST),
    digitalRead(BUTTON_NEXT));
  if(digitalRead(BUTTON_NEXT) == LOW)
  {
    ota_update = true;
    Serial.println(F("FIRMWARE UPDATE requested!"));
  }
  if( digitalRead(BUTTON_LAST) == LOW )
  {
    Serial.println(F("SHOW CONFIG requested!"));
    show_config = true;
  }
  if( (digitalRead(BUTTON_LAST) == LOW) && (digitalRead(BUTTON_NEXT) == LOW) )
  {
    Serial.println(F("AP MODE requested!"));
    force_ap_mode = true;
    show_config = false;
    ota_update = false;
  }

  has_valid_config = WiFiManager_loadConfigData();
  myEpaper.init();
  myBatt.init();
  myBatt.test();
  myBatt.read();
  if(has_valid_config == false)
  {
    Serial.printf("Config status [%d]\n", has_valid_config);  
  }
  new_firmware = hch_check_for_new_fw(FW_VERSION_MAJ, FW_VERSION_MIN);
  DUMP1(new_firmware);
  DUMP1(FW_VERSION_MAJ);
  DUMP1(FW_VERSION_MIN);
  if(new_firmware == true)
  {
    myEpaper.firmware_change(FW_VERSION_MAJ, FW_VERSION_MIN);
  }

/*
  if(force_ap_mode == true)
  {
    myEpaper.ap_mode("Button Pressed");
  }
  */
 
  wasConnected = true;

  WiFiManager_init(CLIENT_ID, force_ap_mode);
  bool new_online_fw = checkVersion();
  if(ota_update == true)
  {
    hch_clear_ota_request();
    if(new_online_fw == true)
    {
      myEpaper.show_ota_update(theVersion.new_major, theVersion.new_minor);
      checkForUpdates();
      Serial.println(F("New Update available\n"));
      return;    
    }
    else
    {
      myEpaper.show_firmware();
      Serial.println(F("Firmware up to date .... continue!\n"));
    }
  }
  setup_button_click();
  TRACE1();
  if(show_config == true)
  {
    myEpaper.show_config();
    Serial.println(F("setup() long timeout"));
  }
  else
  {
    if(has_valid_config == true)
    {
      bool process_status = process_list(new_online_fw);
      if(process_status == false)
      {
        Serial.println(F("NO TABLE FOUND!"));
        myEpaper.no_conn();
      }
    }
    else
    {
        Serial.println(F("NO CONFIGURATION FOUND!"));
        myEpaper.config_mode();
    }
  }
}

void loop() {
  TRACE2();
  main_loop_ctr++;
  WiFiManager_loop();
  button1.loop();
  button2.loop();
  check_sleep();
}
