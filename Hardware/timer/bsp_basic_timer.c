///********************************************************************************
//  * �� �� ��: bsp_basic_timer.c
//  * �� �� ��: ����
//  * �޸�����: LC
//  * �޸�����: 2022��04��18��
//  * ���ܽ���:
//  ******************************************************************************
//  * ע������:
//*********************************************************************************/
//
//#include "bsp_basic_timer.h"
////#include "bsp_led.h"
//#include "stdio.h"
//#include "lvgl.h"
///************************************************
//�������� �� basic_timer_config
//��    �� �� ������ʱ������
//��    �� �� pre��ʱ��Ԥ��Ƶֵ
//					  per������
//�� �� ֵ �� ��
//��    �� �� LC
//*************************************************/
//void basic_timer_config(uint16_t pre,uint16_t per)
//{
//	/* һ�����ڵ�ʱ��T = 1/f, ��ʱʱ��time = T * ����
//    ��Ԥ��Ƶֵλpre,����λper
//    time = (pre + 1) * (per + 1) / psc_clk
//	*/
//	timer_parameter_struct timere_initpara; 							// ���嶨ʱ���ṹ��
//	/* ����ʱ�� */
//	rcu_periph_clock_enable(BSP_TIMER_RCU); 							// ������ʱ��ʱ��
//	/* CK_TIMERx = 4 x CK_APB1  = 4x50M = 200MHZ */
//	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // ���ö�ʱ��ʱ��
//
//
//	timer_deinit(BSP_TIMER);														  // ��λ��ʱ��
//	/* ���ö�ʱ������ */
//	timere_initpara.prescaler = pre-1;                    //  ʱ��Ԥ��Ƶֵ 0-65535   psc_clk = CK_TIMER / pre
//	timere_initpara.alignedmode = TIMER_COUNTER_EDGE;     // ��Ե����
//	timere_initpara.counterdirection = TIMER_COUNTER_UP;  // ���ϼ���
//	timere_initpara.period = per-1;                       // ����
//	/* �����벶���ʱ��ʹ��  �����˲���ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� */
//	timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // ��Ƶ����
//  /* ֻ�и߼���ʱ������ ����Ϊx�����ظ�x+1�ν����ж� */
//	timere_initpara.repetitioncounter = 0;							  // �ظ������� 0-255
//
//	timer_init(BSP_TIMER,&timere_initpara);								// ��ʼ����ʱ��
//
//	/* �����ж����ȼ� */
//	nvic_irq_enable(BSP_TIMER_IRQ,3,2); 									// �����ж����ȼ�Ϊ 3,2
//	/* ʹ���ж� */
//	timer_interrupt_enable(BSP_TIMER,TIMER_INT_UP);       // ʹ�ܸ����¼��ж�
//	/* ʹ�ܶ�ʱ�� */
//	timer_enable(BSP_TIMER);
//}
//
///************************************************
//�������� �� BSP_TIMER_IRQHandler
//��    �� �� ������ʱ���жϷ�����
//��    �� �� ��
//�� �� ֵ �� ��
//��    �� �� LC
//*************************************************/
//void BSP_TIMER_IRQHANDLER(void)
//{
//	/* �����Ƕ�ʱ���ж� */
//  if(timer_interrupt_flag_get(BSP_TIMER,TIMER_INT_FLAG_UP) == SET)
//	{
//		timer_interrupt_flag_clear(BSP_TIMER,TIMER_INT_FLAG_UP);  // ����жϱ�־λ
//		/* ִ�й��� */
//		lv_tick_inc(1);
//	}
//}
