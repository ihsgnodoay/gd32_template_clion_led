#include "gd32f4xx.h"
#include "encoder.h"
#include <stdio.h>
#include "systick.h"

int g_encoder = 0;

/**
PB14 TIMER11_CH0
PB15  TIMER11_CH1

PB4	 TIMER2_CH0
PB5  TIMER2_CH1
*/
void encoder_gpio_init(){
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);
	//PA15  PB3
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_15);
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO_PIN_15);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	
	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_3);
	gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
	//PB4,5
	gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_4|GPIO_PIN_5);
	gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_4|GPIO_PIN_5);
	
}

void encoder_timer_init(){
		timer_parameter_struct timer_initpara;
    timer_ic_parameter_struct timer_icinitpara;
		
		rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /* TIM2 clock enable */
    rcu_periph_clock_enable(RCU_TIMER2);
    
    timer_deinit(TIMER2);

    timer_initpara.period = 65535;
    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);

    /* TIMER3 CH0 input capture configuration */
    timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;//改参数设置对于编码器不起作用
    timer_icinitpara.icfilter    = 0x05;
    timer_input_capture_config(TIMER2, TIMER_CH_0, &timer_icinitpara);
    timer_input_capture_config(TIMER2, TIMER_CH_1, &timer_icinitpara);

    timer_quadrature_decoder_mode_config(TIMER2, TIMER_ENCODER_MODE2, TIMER_IC_POLARITY_FALLING, TIMER_IC_POLARITY_RISING);
		//timer_slave_mode_select(TIMER2,TIMER_ENCODER_MODE2);
		
    //timer_interrupt_enable(TIMER2, TIMER_INT_UP);
		//timer_interrupt_enable(TIMER2, TIMER_INT_CH0);
    //nvic_irq_enable(TIMER2_IRQn, 1, 1);//使能中断并设置优先级
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);
		

    rcu_periph_clock_enable(RCU_TIMER1);
    
    timer_deinit(TIMER1);

    timer_initpara.period = 65535;
    timer_initpara.prescaler = 0;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

   
    timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;//改参数设置对于编码器不起作用
    timer_icinitpara.icfilter    = 0x05;
    timer_input_capture_config(TIMER1, TIMER_CH_0, &timer_icinitpara);
    timer_input_capture_config(TIMER1, TIMER_CH_1, &timer_icinitpara);

    timer_quadrature_decoder_mode_config(TIMER1, TIMER_ENCODER_MODE2, TIMER_IC_POLARITY_FALLING, TIMER_IC_POLARITY_RISING);
		
    timer_auto_reload_shadow_enable(TIMER1);
    timer_enable(TIMER1);
}


void encoder_init(void)
{
		encoder_gpio_init();
		encoder_timer_init();
}

int get_encoder(){
	short enc = TIMER_CNT(TIMER2);
	TIMER_CNT(TIMER2) = 0;
	g_encoder += enc;
	
	printf("enc:%d\r\n",g_encoder);
	return g_encoder;
}

void encoder_clear(){
	TIMER_CNT(TIMER2) = 0;
	g_encoder = 0;
}


/*
float get_right_speed(){
	static int pre_encoder2 = 0;
	static uint32_t pre_time2 = 0;
	
	
	float delta_time = 0;//get_systick_ms() - pre_time2;
	//pre_time2 = get_systick_ms();
	
	
	int enc2 = get_encoder2();
	int diff1 = enc2 - pre_encoder2;
	
	// 线数*上升沿+下降沿*AB两厢*减速比
	// 13*2*2*30
	
	// 转速 rpm 圈每分钟   diff/(13*2*2*30)/时间
	float time = (float)delta_time/1000;
	float quan = (float)diff1/(13*2*2*30);	
	float rpm = quan/time;
	
	
	
	printf("encoder right: diff=%d  cur=%d pre=%d rpm=%02f m/s\n", diff1,enc2,pre_encoder2,rpm);
	
	pre_encoder2 = enc2;
	TIMER_CNT(TIMER3) = 0;
	return rpm;
}



void encoder_test(){
	encoder_init();
	
	uint32_t pre_time = get_systick_ms();
	while(1){
		
		uint32_t delta_time = get_systick_ms() - pre_time;
		short enc1 = get_encoder1();
		short enc2 = get_encoder2();
		
		printf("enc1:%d  enc2:%d\r\n",enc1,enc2);
		//get_left_speed();
		//get_right_speed();
		delay_1ms(100);
	}
}

*/


//void TIMER2_IRQHandler(void)
//{	
//		

//    if (SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
//    {
//				
//        /* 清除更新中断标志 */
//        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
//        //LED1 = !LED1;
//    }
////		else  if (SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_CH0))
////    {
////        /* 清除更新中断标志 */
////        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_CH0);
////        //LED1 = !LED1;
////    }
////		else  if (SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_CH1))
////    {
////        /* 清除更新中断标志 */
////        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_CH1);
////        //LED1 = !LED1;
////    }
//}


//void TIMER33_IRQHandler(void)
//{

//    if (SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
//    {
//        /* 清除更新中断标志 */
//        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
//        //LED1 = !LED1;
//    }
//}