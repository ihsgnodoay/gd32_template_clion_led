#include "battery.h"
#include "systick.h"
#include "stdio.h"
uint16_t 	batteryValue;
#define BATTERY_ADC_RCU RCU_ADC0
#define BATTERY_ADC_PORT ADC0
#define BATTERY_ADC_CHANNEL ADC_CHANNEL_15

void battery_adc_config(void){

		 /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
    /* reset ADC */
    adc_deinit();
    /* configure the ADC  mode */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // 所有ADC都工作在独立模式
    
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // 关闭连续模式
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // 关闭扫描模式
    
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB对齐，低位对齐
    
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,1U); //  ADC规则通道 长度为1
		
		adc_inserted_channel_config(ADC0, ADC_INSERTED_CHANNEL_0, ADC_CHANNEL_15, ADC_SAMPLETIME_144);
		
		adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL,ENABLE);
    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);  // ADC校准
    /* wait for ADC stability */
    delay_1ms(1);
		
		/* adc 引脚初始化 */
		battery_adc_gpio_init();
}

/* ADC的附加功能 优先于普通GPIO  */
 void battery_adc_gpio_init(void)
{
	
		/* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_5);
}

void battery_adc_scan(void){
	batteryValue = battery_adc_sample(BATTERY_ADC_CHANNEL);  // 采样
}

/*!
    \brief      ADC channel sample  ADC通道采样
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t battery_adc_sample(uint8_t channel){
   /* ADC regular channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15个采样周期 
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); // ADC软件触发使能

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_routine_data_read(ADC0));
}
float bsp_battery_get_voltage(){
	 /* ADC software trigger enable */
	adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
	/* delay a time in milliseconds */
	// 等待转换完成
	//while(!adc_flag_get(ADC0,ADC_FLAG_EOIC));
	
	// 清楚转换完成标志位
	//adc_flag_clear(ADC0,ADC_FLAG_EOIC);
	
	uint16_t value = adc_inserted_data_read(ADC0,ADC_INSERTED_CHANNEL_0);
	/* 数据转换  */
	float vref_value = (value * 3.3 / 4096*5);
	
	return vref_value;
}



void battery_test_adc_run(void){
	
	printf("app start\n");
	battery_adc_config();
	//battery_adc_scan();
  while(1){
		float v = bsp_battery_get_voltage();
		printf("batteryValue=%f\n",v);
		delay_1ms(500);
  }
}
