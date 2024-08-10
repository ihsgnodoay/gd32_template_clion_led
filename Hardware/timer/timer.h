#ifndef TIMER_H
#define TIMER_H

#include "gd32f4xx.h"
#include "systick.h"

#define PRESCALER	(10 - 1)
#define PERIOD		(SystemCoreClock / 20000 - 1)

void timer5_config(void);

void timer1_config(void);

void timer2_config(void);

void pwm_update_timer1_ch0(float duty_cycle);

void pwm_update_timer2_ch0(float duty_cycle);

void pwm_update_timer2_ch1(float duty_cycle);

void pwm_update_timer2_ch2(float duty_cycle);

#endif  /* TIMER_H */
