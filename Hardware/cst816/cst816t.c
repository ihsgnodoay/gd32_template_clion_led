#include "cst816t.h"

#include <stdio.h>
#include "i2c.h"
#include "systick.h"



uint8_t State=CST816T_STATE_RELEASE;
uint16_t X_Axis=0;
uint16_t Y_Axis=0;
uint8_t fingerNum = 0;

void CST816T_GPIOInit(void)
{

	//PA_6(RST)
  rcu_periph_clock_enable(CST816T_RESET_RCU);
	gpio_mode_set(CST816T_RESET_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CST816T_RESET_GPIO_PIN);
	gpio_output_options_set(CST816T_RESET_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, CST816T_RESET_GPIO_PIN);
	//PA_7(INT)
	rcu_periph_clock_enable(CST816T_IRQ_RCU);
	gpio_mode_set(CST816T_IRQ_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, CST816T_IRQ_GPIO_PIN);
	
	
		//1.2 开启系统配置时钟
	rcu_periph_clock_enable(RCU_SYSCFG); 
	
	/*3.使能NVIC中断并配置优先级*/
	//3.1 设置中断优先级分组(只需要配置一次  可以放在main中)
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	//3.2 配置中断的抢占优先级和响应优先级 抢占优先级3,子优先级3
	nvic_irq_enable(EXTI3_IRQn,3,3);
	/*4.配置GPIO中断 中断引脚资源端口和中断引脚*/
	syscfg_exti_line_config(EXTI_SOURCE_GPIOC,EXTI_SOURCE_PIN3);
	/*5.初始化中断线  第三个参数指定上升沿和下降沿均触发*/
	exti_init(CST816T_EXTI_Line,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
	/*6.使能中断和清除中断标志位*/
	//清除中断标志位
	exti_interrupt_flag_clear(CST816T_EXTI_Line);
	//使能中断
	exti_interrupt_enable(CST816T_EXTI_Line);
	/*7.编写中断服务函数*/

	//初始化
	_CST816T_RESET_HIGH_();
}


void CST816T_SendByte(uint8_t Addr,uint8_t* Data)
{
	/**
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2A);
	I2C_SOFTWARE_SendByte(Addr);
	I2C_SOFTWARE_SendByte(*Data);
	I2C_SOFTWARE_StopSignal();*/
	//IIC_Send_Byte(0x15,Addr,*Data);
	I2C_write(0x15, Addr,Data, 1);
}

void CST816T_ReceiveByte(uint8_t Addr,uint8_t* Data)
{
	//*Data = IIC_Read_Byte(0x2A,Addr);
	//*Data = IIC_Read_Byte(0x15,Addr);
	I2C_read(0x15, Addr,Data, 1);
	//printf("%d\r\n",*Data);
	/**
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2A);
	I2C_SOFTWARE_SendByte(Addr);
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2B);
	*Data=I2C_SOFTWARE_ReceiveByte(Nack);
	I2C_SOFTWARE_StopSignal();*/
}

void CST816_GetAction(void)
{
	uint8_t data[6];uint16_t X=0,Y=0;
	
	
	CST816T_ReceiveByte(0x01,data);
	CST816T_ReceiveByte(0x03,data+1);
	CST816T_ReceiveByte(0x04,data+2);
	CST816T_ReceiveByte(0x05,data+3);
	CST816T_ReceiveByte(0x06,data+4);
	// 读手指的个数
	CST816T_ReceiveByte(0x02,data+5);
	
	State=data[0];
	X=(uint16_t)((data[1]&0x0F)<<8)|data[2];//(temp[0]&0X0F)<<4|
	Y=(uint16_t)((data[3]&0x0F)<<8)|data[4];//(temp[2]&0X0F)<<4|
	
	if(X<TOUCH_W&&Y<TOUCH_H)
	{
		X_Axis=X;
		Y_Axis=Y;
	}
	
	// 手指的数量
	fingerNum = data[5];
	printf("Sta:%X,X:%d,Y:%d finger:%d \t\n",State,X_Axis,Y_Axis,fingerNum); 	
}

void CST816T_Reset(void)
{
	_CST816T_RESET_LOW_();
	delay_1ms(10);
	_CST816T_RESET_HIGH_();
	delay_1ms(50);
}

void CST816T_Init(void)
{
	uint8_t ChipID=0;
	uint8_t FwVersion=0;
	
	CST816T_GPIOInit();
	CST816T_Reset( );//芯片上电初始化
	
	// 延时
	delay_1ms(100);
	
	CST816T_ReceiveByte(0xa7,&ChipID);
	CST816T_ReceiveByte(0xa9,&FwVersion);
	printf("ChipID:%X\r\n",ChipID);
	printf("FwVersion:%d\r\n",FwVersion);
	CST816T_Reset();
}

void CST816T_ClrBaseDatas(void)
{
	State=CST816T_STATE_RELEASE;
	X_Axis=0;
	Y_Axis=0;
	fingerNum=0;
}



void EXTI3_IRQHandler(void)
{
	 
	 if(exti_interrupt_flag_get(CST816T_EXTI_Line) == SET) {
			exti_interrupt_flag_clear(CST816T_EXTI_Line);
			CST816_GetAction();
			//printf("touch down\r\n");
	 }else{
		  CST816_GetAction();
			//CST816T_ClrBaseDatas();
		  //printf("touch release\r\n");
	 }
	 
	 exti_interrupt_flag_clear(CST816T_EXTI_Line); 
}

uint8_t CST816T_is_pressed(void){
	
	//CST816_GetAction();
	
	if(fingerNum > 0 && X_Axis>0 && Y_Axis>0){
		return 1;
	}else{
		return 0;
	}
}


void CST816T_get_xy(uint16_t* x,uint16_t* y){
	*x = X_Axis;
	*y = Y_Axis;
}