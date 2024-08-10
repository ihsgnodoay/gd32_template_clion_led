#include "timer.h"
#include <stdio.h>
#include "gpio_config.h"

void timer1_config(void)
{
	gpio_config_PA5();

	rcu_periph_clock_enable(RCU_TIMER1);
	timer_deinit(TIMER1);
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	timer_parameter_struct tps;
	timer_struct_para_init(&tps);

	tps.prescaler = PRESCALER;
	tps.period = PERIOD;

	timer_init(TIMER1, &tps);

	timer_oc_parameter_struct tops;
	timer_channel_output_struct_para_init(&tops);
	tops.outputstate = TIMER_CCX_ENABLE;
	timer_channel_output_config(TIMER1, TIMER_CH_0, &tops);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);

	timer_enable(TIMER1);
}

/*!
 * @brief ���ö�ʱ��
 * @param[in]  none
 * @param[out] none
 * @retval     none
 */
void timer5_config()
{
	gpio_config_PD5();

	/* ʱ������ */
	rcu_periph_clock_enable(RCU_TIMER5);
	/* ��λ��ʱ�� */
	timer_deinit(TIMER5);

	/* ��ʱ����Ƶ���� */
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	timer_parameter_struct tps;
	timer_struct_para_init(&tps);

	/* ��Ƶϵ��  240000000 */
	tps.prescaler = PRESCALER;
	/* ���ڼ��� 16bit�����ܳ���65535����˱���������С�� SystemCoreClock / 65535 */
	tps.period = PERIOD;
	/* ��ʼ�� */
	timer_init(TIMER5, &tps);
	nvic_irq_enable(TIMER5_DAC_IRQn, 2, 2);
	timer_interrupt_enable(TIMER5, TIMER_INT_UP);
	/* �� */
	timer_enable(TIMER5);
}

/*!
 * @brief ����ռ�ձ�
 * \param duty_cycle ռ�ձ�
 */
void pwm_update_timer1_ch0(float duty_cycle)
{
	uint32_t pulse = ((PERIOD + 1) * (uint32_t) duty_cycle) / 100 - 1;
	// ����ռ�ձ�
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, pulse);
}