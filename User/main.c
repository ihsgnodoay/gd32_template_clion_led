#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "uart.h"
#include "gpio_config.h"
#include "timer.h"

/*!
 * @brief 定时器Timer5
 */
void TIMER5_DAC_IRQHandler(void)
{
	if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_UP)) {
//		gpio_bit_toggle(GPIOD, GPIO_PIN_5);
		printf("Timer init PD5\r\n");
	}

	timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
}

/*!
 *  @brief main function
 *	@param[in]  none
 *	@param[out] none
 *	@retval     none
*/
int main(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

	systick_config();

	uart_config();

	timer5_config();

//	timer1_config();

	while (1) {}
}
