//
// Created by syd on 2024/7/23.
//

#include "gpio_config.h"
#include "gd32f4xx.h"
#include "systick.h"

void gpio_config_PA3(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);
	gpio_bit_reset(GPIOA, GPIO_PIN_3);
}

void gpio_config_PA5(void)
{

	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5);
}

/*!
 * @brief TIMER2_CH_0
 */
void gpio_config_PC6(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6);
	gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_6);
}

/*!
 * @brief TIMER2_CH_1
 */
void gpio_config_PC7(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);
	gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_7);
}

/*!
 * @brief TIMER2_CH_2
 */
void gpio_config_PC8(void)
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
	gpio_af_set(GPIOC, GPIO_AF_2, GPIO_PIN_8);
}

void gpio_config_PD5(void)
{
	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
}