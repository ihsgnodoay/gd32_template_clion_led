#include "motor.h"
#include "gd32f4xx.h"
#include <math.h>
#include "systick.h"
/**
TIMER4_CH0 PA0
TIMER4_CH1 PA1
TIMER4_CH2 PA2
TIMER4_CH3 PA3
*/
void motor_gpio_init(){
	rcu_periph_clock_enable(RCU_GPIOA);
	
		// TIMER4_CH2 PA2
	gpio_af_set(GPIOA, GPIO_AF_2,GPIO_PIN_2);
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_2);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_bit_set(GPIOA,GPIO_PIN_2);
	
	// TIMER4_CH2 PA3
	gpio_af_set(GPIOA, GPIO_AF_2,GPIO_PIN_3);
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_3);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	gpio_bit_reset(GPIOA,GPIO_PIN_3);


}



void motor_timer_init(){
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
	timer_parameter_struct Timer_ParamType;
	timer_oc_parameter_struct Timer_OC_ParamType;
	timer_break_parameter_struct Timer_BreakParaType;
	
	rcu_periph_clock_enable(RCU_TIMER4);
	
	timer_deinit(TIMER4);
	timer_struct_para_init(&Timer_ParamType);
	
	Timer_ParamType.alignedmode = TIMER_COUNTER_EDGE;//����Ϊ���ض���ģʽ
	Timer_ParamType.clockdivision = TIMER_CKDIV_DIV1;//��������ʱ��Ͳ���ʱ�ӣ�DTS��֮��ķ�Ƶϵ��
	Timer_ParamType.counterdirection = TIMER_COUNTER_UP;//���ü�������Ϊ���ϼ���
	Timer_ParamType.period = 7200-1;//����
	Timer_ParamType.prescaler = 200-1;//Ԥ��Ƶ
	Timer_ParamType.repetitioncounter = 0;
	timer_init(TIMER4,&Timer_ParamType);
	

	timer_channel_output_struct_para_init(&Timer_OC_ParamType);
	
	//Timer_OC_ParamType.ocidlestate = TIMER_OC_IDLE_STATE_HIGH;//����ʱͨ���������
	//Timer_OC_ParamType.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;//����ʱ����ͨ���������
	//Timer_OC_ParamType.ocnpolarity = TIMER_OCN_POLARITY_HIGH;//����ͨ���������
	Timer_OC_ParamType.ocpolarity = TIMER_OC_POLARITY_HIGH;//ͨ���������
	//Timer_OC_ParamType.outputnstate = TIMER_CCXN_DISABLE;//ʹ�ܻ���ͨ�����
	Timer_OC_ParamType.outputstate = TIMER_CCX_ENABLE;//ʹ��ͨ�����
	
	

	timer_channel_output_config(TIMER4,TIMER_CH_2,&Timer_OC_ParamType);
	timer_channel_output_config(TIMER4,TIMER_CH_3,&Timer_OC_ParamType);
	
	
	timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_2, 0);//�ȽϼĴ�������ֵ
	timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_3, 0);//�ȽϼĴ�������ֵ
	
	

	timer_channel_output_mode_config(TIMER4,TIMER_CH_2,TIMER_OC_MODE_PWM0);//����ͨ�����ģʽ
	timer_channel_output_mode_config(TIMER4,TIMER_CH_3,TIMER_OC_MODE_PWM0);//����ͨ�����ģʽ
	
	
	timer_channel_output_shadow_config(TIMER4,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(TIMER4,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);
	

	
	timer_primary_output_config(TIMER4,ENABLE);
	//timer_auto_reload_shadow_enable(TIMER4);
	timer_enable(TIMER4);

}


int limit_pwm(int pwm){
	if(pwm < -7100){
		pwm = -7100;
	}else if(pwm > 7100){
		pwm = 7100;
	}
	return pwm;
}


void motor520_test(){
	motor_init();
	
	uint16_t pwm = 0;
	
	
	while(1){
		
		//pwm+=100;
		pwm = 7100;
		motor_set_pwm(pwm);

		if(pwm > 6000){
			//pwm = 0;
			delay_1ms(3000);
		}
		delay_1ms(300);
	}
}



void motor_init(){
	// ��ߵ��
	motor_gpio_init();	
	
	
	motor_timer_init();
}


void motor_set_pwm(int pwm){
		pwm = limit_pwm(pwm);
		
		if(pwm > 0){
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_2, 0);
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_3, pwm);
		}else if(pwm < 0){
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_2,	-pwm);
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_3, 0);
		}else{
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_2,	7200);
			timer_channel_output_pulse_value_config(TIMER4, TIMER_CH_3, 7200);
		}
}

