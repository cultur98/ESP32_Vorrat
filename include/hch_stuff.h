#ifndef _HCH_STUFF_H
#define _HCH_STUFF_H
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <esp_spi_flash.h>

#define HEAD "HCHv1"
typedef struct _eeprom_block
{
  char head[10];
  char ssid[20];
  uint32_t boot_ctr;
  uint32_t reboot_ctr;
  uint32_t fw_version_maj;
  uint32_t fw_version_min;
  uint16_t ota_request;
  int language;
} eeprom_block;

extern void esp32_print_wakeup_reason();
extern void esp32_sleep(int);
extern void esp32_shutdown();
extern void IRAM_ATTR esp32_restart();
extern uint32_t esp_32_get_free_heap();

extern bool hch_init(char *name, int speed);
extern bool get_ssid(char *text);
extern void hch_uptime(int seconds, char* string);
extern bool hch_check_for_new_fw(uint32_t, uint32_t);
extern uint32_t hch_get_boot_ctr();
extern bool hch_set_ota_request();
extern int hch_get_lang();
extern bool hch_set_lang(int);
extern bool hch_clear_ota_request();
extern void i2c_scanner(TwoWire &wirePort);
extern void esp_32_restart();
#endif
