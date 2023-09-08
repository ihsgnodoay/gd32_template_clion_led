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
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // ����ADC�������ڶ���ģʽ
    
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // �ر�����ģʽ
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // �ر�ɨ��ģʽ
    
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB���룬��λ����
    
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,1U); //  ADC����ͨ�� ����Ϊ1
		
		adc_inserted_channel_config(ADC0, ADC_INSERTED_CHANNEL_0, ADC_CHANNEL_15, ADC_SAMPLETIME_144);
		
		adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL,ENABLE);
    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);  // ADCУ׼
    /* wait for ADC stability */
    delay_1ms(1);
		
		/* adc ���ų�ʼ�� */
		battery_adc_gpio_init();
}

/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
 void battery_adc_gpio_init(void)
{
	
		/* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_5);
}

void battery_adc_scan(void){
	batteryValue = battery_adc_sample(BATTERY_ADC_CHANNEL);  // ����
}

/*!
    \brief      ADC channel sample  ADCͨ������
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t battery_adc_sample(uint8_t channel){
   /* ADC regular channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15���������� 
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL); // ADC�������ʹ��

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
	// �ȴ�ת�����
	//while(!adc_flag_get(ADC0,ADC_FLAG_EOIC));
	
	// ���ת����ɱ�־λ
	//adc_flag_clear(ADC0,ADC_FLAG_EOIC);
	
	uint16_t value = adc_inserted_data_read(ADC0,ADC_INSERTED_CHANNEL_0);
	/* ����ת��  */
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
