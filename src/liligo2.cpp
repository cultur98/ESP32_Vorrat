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

#include "ePaperBWR.h"
#include "lilygo.h"

#if defined LILYGO_21 || defined LILYGO_27 || defined LILYGO_29
#define ENABLE_GxEPD2_GFX 0
#include <Arduino.h>
#include <Wire.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>


#ifdef LILYGO_21
GxEPD2_3C<GxEPD2_213c, GxEPD2_213c::HEIGHT> display(GxEPD2_213c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
#endif

#ifdef LILYGO_27
GxEPD2_3C<GxEPD2_270c, GxEPD2_270c::HEIGHT> display(GxEPD2_270c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
#endif

#ifdef LILYGO_29
GxEPD2_3C<GxEPD2_290c, GxEPD2_290c::HEIGHT> display(GxEPD2_290c(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));
#endif

#include "GxEPD2_boards_added.h"

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;


#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"
#include "Logger.h"
#include "wifiman.h"

void EPD_init(int &width, int &height)
{
  TRACE1();
  display.init();
  height = display.height();
  width = display.width();
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  display.setFullWindow();
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_RED);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);  // apply Adafruit GFX color
}

void EPD_draw_logo()
{
}

void EPD_power_off()
{}

void EPD_orient(int orientation)
{
  display.setRotation(orientation);
}


void EPD_enter()
{
  TRACE1();

  display.firstPage();
}

void EPD_leave()
{
  TRACE1();
  display.nextPage();
}

void EPD_draw_rect(int x_pos, int y_pos, int rec_width, int rec_height)
{
  Serial.printf("rect (%d|%d) [%d,%d]\n", x_pos, y_pos, rec_width, rec_height);
  display.fillRect(x_pos, y_pos, rec_width, rec_height, GxEPD_WHITE);
}

void EPD_print_text(int font_size, char *text, int x_pos, int y_pos, int style)
{
  TRACE2();
  if(style == _FONT_BOLD_)
    u8g2Fonts.setForegroundColor(GxEPD_RED);
  else
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
  switch (font_size)
  {
  case 8:
    u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    break;
  
  case 12:
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    break;

  default:
    u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    font_size = 10;
    break;
  }
  u8g2Fonts.setCursor(x_pos, y_pos);
#if ARDUINOTRACE_ENABLE > 1
  Serial.printf("EPD_print_text (%d|%d) [%d,%d] (%s) \n", font_size, bold, x_pos, y_pos, text);
#endif
  u8g2Fonts.println(text);
}

#endif