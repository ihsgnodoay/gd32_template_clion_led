//
// Created by syd on 2024/7/23.
//

#include "gpio_config.h"
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

void gpio_config_PD5(void)
{
	rcu_periph_clock_enable(RCU_GPIOD);

	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);

}
