// Based on
// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#ifdef LILYGO_47
#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "epd_driver.h"
#include "esp_adc_cal.h"
#include "lilygo.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "vorrat_logo.h"

#include "RobotoCondensed_14.h"
#include "RobotoCondensed_16.h"
#include "RobotoCondensed_18.h"
#include "RobotoCondensed_bold_14.h"
#include "RobotoCondensed_bold_16.h"
#include "RobotoCondensed_bold_18.h"
#include "RobotoCondensed_light_14.h"
#include "RobotoCondensed_light_16.h"
#include "RobotoCondensed_light_18.h"

#define BATT_PIN            36
#define SD_MISO             12
#define SD_MOSI             13
#define SD_SCLK             14
#define SD_CS               15

uint8_t *framebuffer;
int vref = 1100;
float batt_voltage = 0.0;

#define SMALL_DELAY 50

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"

void EPD_init(int &width, int &height)
{
  TRACE1();
  // Correct the ADC reference voltage

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
      vref = adc_chars.vref;
  }

  epd_init();
  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
      Serial.println(F("alloc memory failed !!!"));
      while (1);
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  width = EPD_WIDTH;
  height = EPD_HEIGHT;
}

void EPD_draw_logo()
{
  Rect_t area = {
      .x = 600,
      .y = 200,
      .width = logo_width,
      .height =  logo_height
  };
  epd_draw_grayscale_image(area, (uint8_t *) logo_data);
}

void EPD_orient(int orientation)
{
}

void EPD_enter()
{
  TRACE1();
  epd_poweron();
  epd_clear();
  uint16_t v = analogRead(BATT_PIN);
  batt_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
  Serial.println(batt_voltage);

  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  epd_clear();
  epd_fill_rect(0, 0, EPD_WIDTH, EPD_HEIGHT, 255, framebuffer);
  vTaskDelay(2*SMALL_DELAY);
}

void EPD_leave()
{
  TRACE1();
  epd_draw_grayscale_image(epd_full_screen(), framebuffer);
  TRACE1();
  vTaskDelay(2*SMALL_DELAY);
  epd_draw_grayscale_image(epd_full_screen(), framebuffer);
  TRACE1();
  vTaskDelay(5*SMALL_DELAY);
  epd_poweroff();
  TRACE1();
}

void EPD_power_off()
{
  TRACE1();
  epd_poweroff_all();
}

void EPD_draw_rect(int x_pos, int y_pos, int width, int height)
{
  TRACE1();
  Serial.printf("rect (%d|%d) [%d,%d]\n", x_pos, y_pos, width, height);
  epd_fill_rect(x_pos, y_pos, width, height, 255, framebuffer);
}


void EPD_print_text(int font_size, char *text, int x_pos, int y_pos, int style)
{
  TRACE2();
  GFXfont* lightFont;
  GFXfont* normalFont;
  GFXfont* boldFont;
  switch (font_size)
  {
  case 14:
    lightFont = (GFXfont *)&RobotoCondensed_light_14;
    normalFont = (GFXfont *)&RobotoCondensed_14;
    boldFont = (GFXfont *)&RobotoCondensed_bold_14;
    break;

  case 18:
    lightFont = (GFXfont *)&RobotoCondensed_light_18;
    normalFont = (GFXfont *)&RobotoCondensed_18;
    boldFont = (GFXfont *)&RobotoCondensed_bold_18;
    break;

  default:
    lightFont = (GFXfont *)&RobotoCondensed_light_16;
    normalFont = (GFXfont *)&RobotoCondensed_16;
    boldFont = (GFXfont *)&RobotoCondensed_bold_16;
    break;
  }
#if ARDUINOTRACE_ENABLE > 1
  Serial.printf("show (%d|%d) [%d,%d] {%d} ""%s"" \n", font_size, bold, x_pos, y_pos, font_size, text);
#endif
  if( (x_pos >= 0) && (x_pos < EPD_WIDTH) && (y_pos >= 0) && (y_pos < EPD_HEIGHT) )
  {
    switch (style)
    {
    case _FONT_LIGHT_:
      write_string(lightFont, text, &x_pos, &y_pos, framebuffer);
      break;
    
    case _FONT_BOLD_:
      write_string(boldFont, text, &x_pos, &y_pos, framebuffer);
      break;

    default:
      write_string(normalFont, text, &x_pos, &y_pos, framebuffer);
      break;
    }
  }
  else
  {
    Serial.println(F(" coordinates out of bounds .."));
  }
}
#endif