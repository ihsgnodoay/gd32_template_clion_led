#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "uart.h"
#include "gpio_config.h"


/*!
 *	@brief 接受数据
 */
void usart0_recv(uint8_t* data, uint32_t len)
{
	printf(">> %s\r\n", data);
}

static void timer_config() {
	// 时钟配置
	rcu_periph_clock_enable(RCU_TIMER5);

	// 复位定时器
	timer_deinit(TIMER5);

	/* 定时器倍频操作 */
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

	timer_parameter_struct tps;
	timer_struct_para_init(&tps);

	/* 分频系数  240000000 */
	tps.prescaler = 10000 - 1;

	/* 周期计数 */
	tps.period = SystemCoreClock / 20000 - 1;

	/* 初始化 */
	timer_init(TIMER5, &tps);

	nvic_irq_enable(TIMER5_DAC_IRQn, 2, 2);
	timer_interrupt_enable(TIMER5, TIMER_INT_UP);

	/* 打开 */
	timer_enable(TIMER5);
}

void TIMER5_DAC_IRQHandler(void)
{
	if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_UP)) {
		gpio_bit_toggle(GPIOD, GPIO_PIN_5);
	}


	timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
}


/*!
 *  @brief		main function
 *	@param[in]  none
 *	@param[out] none
 *	@retval     none
*/
int main(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	systick_config();

	uart_gpio_init_config();
	uart_config();

	timer_config();

	gpio_config_PD5();

	while (1) {
		delay_1ms(100);
	}
}
