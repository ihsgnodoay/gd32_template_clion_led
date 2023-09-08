#ifndef __BATTERY__H__
#define __BATTERY__H__
#include "gd32f4xx.h"
#include <stdint.h>

void battery_adc_gpio_init(void);
void battery_adc_config(void);
uint16_t battery_adc_sample(uint8_t channel);
void battery_adc_scan(void);

void battery_test_adc_run(void);

float bsp_battery_get_voltage();

#endif