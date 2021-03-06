#include "batty.h"
#include "Config.h"
#include "esp_adc_cal.h"

#define ARDUINOTRACE_ENABLE 1
#include "ArduinoTrace.h"

float batt_voltage;
uint8_t batt_level;

Batty::Batty() {
  batt_voltage = 0.0f;
  batt_level = 0;
}

// copied from https://github.com/rlogiacco/BatterySense
void Batty::init() {
  TRACE1();
  vref = 1100;
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.printf("eFuse Vref:%u mV\n", adc_chars.vref);
      vref = adc_chars.vref;
  }
}

uint8_t Batty::level()
{
  uint16_t maxVoltage = MAX_VOLTAGE;
  uint16_t minVoltage = MIN_VOLTAGE;
  float batt_volt = batt_voltage;
  if(batt_voltage <= ((float)MIN_VOLTAGE/1000.0f))
    batt_volt = ((float)MIN_VOLTAGE+1)/1000.0f;
  if(batt_voltage > ((float)MAX_VOLTAGE/1000.0f))
    batt_volt = (((float)MAX_VOLTAGE+1)/1000.0f);
  DUMP1(batt_voltage);
  DUMP1(batt_volt);
  // copied from https://github.com/rlogiacco/BatterySense
  batt_level = 105 - (105 / (1 + pow(1.724 * (uint16_t(batt_volt*1000.0f) - minVoltage)/(maxVoltage - minVoltage), 5.5)));
  return(batt_level);
}

void Batty::test()
{
  TRACE1();
  for(batt_voltage = 2.001; batt_voltage < 5.0; batt_voltage = batt_voltage + 0.05f)
  {
    int test_level = level();
    Serial.printf("Vtest: %.3fV\t%d\t%d\n", batt_voltage, batt_level, test_level);
  }
}

void Batty::read()
{
  TRACE1();
  Serial.printf("Battery voltage from pin %d is ", BAT_SENSE_PIN);
  uint16_t v = analogRead(BAT_SENSE_PIN);
  batt_voltage = ((float)v / 4095.0) * BAT_GAIN * 3.3 * (vref / 1000.0);
  Serial.print(batt_voltage);
  Serial.print("V (");
  batt_level = level();
  Serial.print(batt_level);
  Serial.println("%)");
}
