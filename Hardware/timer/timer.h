#ifndef _TIMER_H
#define _TIMER_H

#include "gd32f4xx.h"
#include "systick.h"

#define PRESCALER   (10000 -1)
#define PERIOD      (SystemCoreClock / 20000 - 1)

void timer5_config();

void timer1_config();

void pwm_update_timer1(float duty_cycle);

#endif  /* _TIMER_H */
