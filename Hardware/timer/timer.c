#include "timer.h"
#include <stdio.h>
#include "gpio_config.h"

/*!
 * @brief 配置定时器
 * @param[in]  none
 * @param[out] none
 * @retval     none
 */
void timer5_config()
{
	gpio_config_PD5();

	/* 时钟配置 */
	rcu_periph_clock_enable(RCU_TIMER5);
	/* 复位定时器 */
	timer_deinit(TIMER5);

	/* 定时器倍频操作 */
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	timer_parameter_struct tps;
	timer_struct_para_init(&tps);

	/* 分频系数  240000000 */
	tps.prescaler = 10000 - 1;
	/* 周期计数 16bit，不能超过65535，因此被除数不能小于 SystemCoreClock / 65535 */
	tps.period = SystemCoreClock / 20000 - 1;
	/* 初始化 */
	timer_init(TIMER5, &tps);
	nvic_irq_enable(TIMER5_DAC_IRQn, 2, 2);
	timer_interrupt_enable(TIMER5, TIMER_INT_UP);
	/* 打开 */
	timer_enable(TIMER5);
}

void timer1_config(void)
{
	gpio_config_PA5();

	rcu_periph_clock_enable(RCU_TIMER1);
	timer_deinit(TIMER1);
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	timer_parameter_struct tps;
	timer_struct_para_init(&tps);

	tps.prescaler = 10000 -1;
	tps.period = SystemCoreClock / 20000 - 1;
	timer_init(TIMER1, &tps);

	timer_oc_parameter_struct tops;
	timer_channel_output_struct_para_init(&tops);
	tops.outputnstate = TIMER_CCXN_ENABLE;
	timer_channel_output_config(TIMER1, TIMER_CH_0, &tops);

//	nvic_irq_enable(TIMER1_IRQn, 2, 2);
//	timer_interrupt_enable(TIMER1, TIMER_INT_UP);

	timer_enable(TIMER1);
}

