#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

/**
 * LED 硬件对应关系
 * LED1 PE3
 * LED2 PD7
 * LED3 PG3
 * LED4 PA5
*/
void gpio_config()
{
	// 1. 时钟初始化
        rcu_periph_clock_enable(RCU_GPIOE);
        // 2. 配置GPIO 输入输出模式
        gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
        // 配置GPIO模式的操作方式
        gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
}


int main(void)
{
        systick_config();
        gpio_config();

        while (1) {
                gpio_bit_set(GPIOE, GPIO_PIN_3);
                delay_1ms(1000);
                gpio_bit_reset(GPIOE, GPIO_PIN_3);
                delay_1ms(1000);
        }
}
