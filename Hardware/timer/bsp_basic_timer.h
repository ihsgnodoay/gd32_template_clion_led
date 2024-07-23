#ifndef _BSP_BASIC_TIMER_H
#define _BSP_BASIC_TIMER_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_TIMER_RCU  				RCU_TIMER5            // ��ʱ��ʱ��
#define BSP_TIMER      				TIMER5                // ��ʱ��
#define BSP_TIMER_IRQ  				TIMER5_DAC_IRQn       // ��ʱ���ж�
#define BSP_TIMER_IRQHANDLER        TIMER5_DAC_IRQHandler // ��ʱ���жϷ�����


//#define BSP_TIMER_RCU  				RCU_TIMER2					// ��ʱ��ʱ��
//#define BSP_TIMER      				TIMER2							// ��ʱ��
//#define BSP_TIMER_IRQ  			  TIMER2_IRQn					// ��ʱ���ж�
//#define BSP_TIMER_IRQHANDLER  TIMER2_IRQHandler		// ��ʱ���жϷ�����


void basic_timer_config(uint16_t pre,uint16_t per); // ������ʱ������

#endif  /* BSP_BASIC_TIMER_H */
