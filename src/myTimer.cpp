#include "Config.h"
#include "myTimer.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP ntp_udp;
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient NTPtimeClient(ntp_udp, NTP_SERVER_NAME);

#define ARDUINOTRACE_ENABLE 2
#include "ArduinoTrace.h"

MyTimer::MyTimer() {
  ntp_is_valid = false;
  ntp_unix_time = 0;
}

void MyTimer::init(int delta){
  TRACE1();
  delta_hours = delta;
}

uint32_t MyTimer::tick(){
  TRACE1();
  DUMP1(ntp_unix_time);
  return(ntp_unix_time);
}

uint32_t MyTimer::getNTP(){
  TRACE1();
  if (WiFi.status() == WL_CONNECTED)
  {
    char buf[80];
    if(ntp_is_valid == false)
    {
      NTPtimeClient.begin();
      if(NTPtimeClient.update() == true)
      {
        ntp_unix_time = NTPtimeClient.getEpochTime();
        ntp_unix_time = ntp_unix_time + (3600 * delta_hours);
        getTimeString(buf, 80, ntp_unix_time);
        DUMP2(ntp_unix_time);
        DUMP2(buf);
        ntp_is_valid = true;
      }
      else
      {
        Serial.println(F("Failed to update NTP!"));    
      }
      NTPtimeClient.end();
    }
  }
  else
  {
    ntp_is_valid = false; 
    Serial.println(F("WiFi not connected!"));
  }
  return(ntp_unix_time);
}

int MyTimer::getMinsOfDay()
{
  time_t     now;
  struct tm  ts;
  int mins = -1;
  char buf[MIN_STRING_LEN];

  if(ntp_unix_time > 0)
  {
    now = ntp_unix_time;
    ts = *localtime(&now);
    strftime(buf, MIN_STRING_LEN, "%d.%m.%y %H:%M", &ts);
    Serial.printf("%s\n", buf);
  }
  return(mins);
}

void MyTimer::getTimeString(char *buf, int bufsize, uint32_t the_time)
{
  time_t     now;
  struct tm  ts;

  now = the_time;
  ts = *localtime(&now);
  strftime(buf, bufsize, "%d.%m.%y %H:%M", &ts);
  Serial.printf("%s\n", buf);
}

bool MyTimer::getTimeStringShort(char *buf, int bufsize)
{
  time_t     now;
  struct tm  ts;

  now = ntp_unix_time;
  ts = *localtime(&now);
  strftime(buf, bufsize, "%H:%M", &ts);
  return (ntp_is_valid);
}

bool MyTimer::getTimeString(char *buf, int bufsize)
{
  time_t     now;
  struct tm  ts;

  now = ntp_unix_time;
  ts = *localtime(&now);
  strftime(buf, bufsize, "%d.%m.%y %H:%M", &ts);
  return (ntp_is_valid);
}

bool MyTimer::update()
{
  getNTP();
  if(ntp_unix_time > 0)
  {
    Serial.println(F("Setting time to NTP time."));
    return(ntp_is_valid);
  }
  DUMP1(ntp_unix_time);
  return(ntp_is_valid);
}

bool MyTimer::isValid()
{
  return (ntp_is_valid);
}