#include "deep_sleep_helper.h"
#include "Config.h"
#include "lilygo.h"

// enable debug messages on the console
#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"

unsigned long start_millis = 0;
unsigned long button_millis = 0;
unsigned long sleep_delay;

void set_timeout(int mode)
{
  DUMP1(mode);
  button_millis = millis();
  switch (mode)
  {
  case MEDIUM_TIMEOUT:
    sleep_delay = MEDIUM_TIMEOUT_SEC * 1000;
    break;
  
  case LONG_TIMEOUT:
    sleep_delay = LONG_TIMEOUT_SEC * 1000;
    break;

  default:
    sleep_delay = SHORT_TIMEOUT_SEC * 1000;
    break;
  } 
}

bool init_wakeup(){
  TRACE1();
  start_millis = button_millis = millis();
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
      Serial.println(F("Wakeup caused by external signal using RTC_IO")); 
      break;
    case ESP_SLEEP_WAKEUP_EXT1 : 
      Serial.println(F("Wakeup caused by external signal using RTC_CNTL")); 
      break;
    case ESP_SLEEP_WAKEUP_TIMER : 
      Serial.println(F("Wakeup caused by timer")); 
      return(true);
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
      Serial.println(F("Wakeup caused by touchpad")); 
      break;
    case ESP_SLEEP_WAKEUP_ULP : 
      Serial.println(F("Wakeup caused by ULP program")); 
      break;
    default : 
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); 
      break;
  }
  return(false);
}

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

void set_deep_sleep() {
  TRACE1();
  EPD_power_off();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
  unsigned long run_millis = millis() - start_millis;
  Serial.printf("Going to sleep now - uptime was %.2fs\n", ((float)run_millis)/1000.0f);
  esp_deep_sleep_start();
  Serial.println(F("This will never be printed"));
}

int get_timeout_delta()
{
  TRACE1();
  unsigned long the_millis;
  the_millis = millis();  
  return((button_millis + sleep_delay - the_millis)/1000); 
}

void check_sleep()
{
  unsigned long the_millis;
  the_millis = millis();  
  if((the_millis - button_millis) > sleep_delay)
  {
    DUMP2(the_millis);
    DUMP2(button_millis);
    DUMP2(sleep_delay);
    Serial.println(F("TICK timeout"));
    set_deep_sleep();
  }
}