#include "hch_stuff.h"
#include "Config.h"

#define EEPROM_OFFSET 1536
#define OTA_IDENTIFIER 31415

eeprom_block the_eeprom_block;
char the_ssid[20];

void set_ssid()
{
  char ssid1[15];
  char ssid2[15];
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  Serial.println(F("Hardware info"));
  Serial.printf("%d cores Wifi %s%s\n", chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
  (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
  Serial.printf("Silicon revision: %d\n", chip_info.revision);
  Serial.printf("%dMB %s flash\n", spi_flash_get_chip_size()/(1024*1024),
  (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embeded" : "external");
  
  //get chip id
  String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
  chipId.toUpperCase();
  
  Serial.printf("Chip id: %s\n", chipId.c_str());

  uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(ssid1, 15, "%04X", chip);
  snprintf(ssid2, 15, "%08X", (uint32_t)chipid);
  snprintf(the_ssid, 19, "ESP32-%04X%08X", chip, (uint32_t)chipid);
  Serial.printf("SSID: %s\n", the_eeprom_block.ssid);
}

bool get_ssid(char *text)
{
  if(strlen(the_eeprom_block.ssid) == 18)
  {
    strcpy(text, the_eeprom_block.ssid);
    return true;
  }
  else
  {
    text[0] = 0;
    return false;
  }
}

bool hch_init(char *name, int speed)
{
  Serial.begin(speed);
  delay(200);
  bool retval = false;
  Serial.println(F("===================="));
  Serial.print("Program : ");
  Serial.println(name);
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.print(", ");
  Serial.println(__TIME__);
  Serial.println(F("--------------------"));
  set_ssid();
  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.println(F("--------------------"));
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    Serial.printf(" head: %s\n", the_eeprom_block.head);
    Serial.printf(" ssid: %s\n", the_eeprom_block.ssid);
    Serial.printf(" bctr: %d\n", the_eeprom_block.boot_ctr);
    Serial.printf(" rctr: %d\n", the_eeprom_block.reboot_ctr);
    Serial.printf(" fw  : %d.%d\n", the_eeprom_block.fw_version_maj, the_eeprom_block.fw_version_min);
    Serial.printf(" ota : %d\n", the_eeprom_block.ota_request);
    Serial.printf(" lang: %d\n", the_eeprom_block.language);
    the_eeprom_block.boot_ctr = the_eeprom_block.boot_ctr + 1;
    if(the_eeprom_block.ota_request == OTA_IDENTIFIER)
      retval = true;
    EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
  }
  else
  {

    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
    Serial.printf("Initializing!\n");
    strcpy(the_eeprom_block.head, HEAD);
    strcpy(the_eeprom_block.ssid, the_ssid);
    the_eeprom_block.boot_ctr = 0;
    the_eeprom_block.reboot_ctr = 0;
    the_eeprom_block.fw_version_maj = 0;
    the_eeprom_block.fw_version_min = 0;
    the_eeprom_block.ota_request = 1;
    EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
  }
  EEPROM.commit();
  EEPROM.end();   
  Serial.printf("portTICK_PERIOD_MS %d\n", portTICK_PERIOD_MS);
  Serial.println(F("===================="));
  return(retval);
}

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

uint32_t hch_get_boot_ctr()
{
  return(the_eeprom_block.boot_ctr);
}

void hch_uptime(int seconds, char* string)
{
  int days=seconds / 86400;
  int hours=(seconds / 3600) - (days * 24);
  int mins=(seconds / 60) - (days * 1440) - (hours * 60);
  int secs=seconds % 60;
  sprintf(string, "%dd [%02d:%02d:%02d]", days, hours, mins, secs);
}
/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void esp32_print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println(F("Wakeup caused by external signal using RTC_IO")); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println(F("Wakeup caused by external signal using RTC_CNTL")); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println(F("Wakeup caused by timer")); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println(F("Wakeup caused by touchpad")); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println(F("Wakeup caused by ULP program")); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void esp32_sleep(int time)
{
  /*
  First we configure the wake up source
  We set our ESP32 to wake up every "time" seconds
  */
  esp_sleep_enable_timer_wakeup(time * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for " + String(time) + " seconds.");
  esp_deep_sleep_start();
}

void IRAM_ATTR esp32_restart(){
  the_eeprom_block.boot_ctr = the_eeprom_block.boot_ctr + 1;
  EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
  Serial.println(F("Restart ESP32"));
  delay(20);
  ESP.restart();
}

void esp32_shutdown()
{
  esp_deep_sleep_start();
}

void i2c_scanner(TwoWire &wirePort)
{
  byte error, address;
  int nDevices;
 
  Serial.println(F("Scanning..."));
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    wirePort.beginTransmission(address);
    error = wirePort.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println(F("  !"));
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print(F("Unknown error at address 0x"));
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found"));
  else
    Serial.println(F("done"));

}

bool hch_check_for_new_fw(uint32_t new_fw_maj, uint32_t new_fw_min)
{
  bool retval = false;
  int old_fw_maj = the_eeprom_block.fw_version_maj;
  int old_fw_min = the_eeprom_block.fw_version_min;

  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    if((new_fw_maj != old_fw_maj) || (new_fw_min != old_fw_min))
    {
      retval = true;
      Serial.printf(" Firmware %d.%d -> %d.%d\n", 
        the_eeprom_block.fw_version_maj, the_eeprom_block.fw_version_min, 
        new_fw_maj, new_fw_min);
      the_eeprom_block.fw_version_maj = new_fw_maj;
      the_eeprom_block.fw_version_min = new_fw_min;
      EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
      EEPROM.commit();
    }
  }
  else
  {
    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
  }
  EEPROM.end();   
  return(retval);
}

bool hch_set_lang(int lang)
{
  bool retval = false;

  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    if(the_eeprom_block.language != lang)
    {
      Serial.printf(" Set language to %d\n", lang);
      the_eeprom_block.language = lang;
      EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
      EEPROM.commit();
    }
    retval = true;
  }
  else
  {
    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
    retval = false;
  }
  EEPROM.end();   
  return(retval);
}

int hch_get_lang()
{
  int lang = -1;

  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    lang = the_eeprom_block.language;
  }
  else
  {
    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
  }
  EEPROM.end();   
  return(lang);
}


bool hch_set_ota_request()
{
  bool retval = false;

  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    Serial.printf(" Set OTA request\n");
    the_eeprom_block.ota_request = OTA_IDENTIFIER;
    EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
    EEPROM.commit();
    retval = true;
  }
  else
  {
    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
    retval = false;
  }
  EEPROM.end();   
  return(retval);
}

bool hch_clear_ota_request()
{
  bool retval = false;

  EEPROM.begin(EEPROM_OFFSET + sizeof(eeprom_block));
  uint16_t eeprom_length = EEPROM.length();
  Serial.printf("EEPROM size: %u\n", (uint32_t)eeprom_length);
  EEPROM.get(EEPROM_OFFSET, the_eeprom_block);
  if(strcmp(the_eeprom_block.head, HEAD) == 0)
  {
    Serial.printf("Found valid EEPROM Header.\n");
    Serial.printf(" Clear OTA request\n"); 
    the_eeprom_block.ota_request = 0;
    EEPROM.put(EEPROM_OFFSET, the_eeprom_block);
    EEPROM.commit();
    retval = true;
  }
  else
  {
    Serial.printf("No valid EEPROM Header found %s\n", the_eeprom_block.head);
    retval = false;
  }
  EEPROM.end();   
  return(retval);
}

void esp_32_restart()
{
  ESP.restart();
}


uint32_t esp_32_get_free_heap()
{
  return(ESP.getFreeHeap());
}