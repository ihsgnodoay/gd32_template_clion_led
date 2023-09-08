#ifndef _cst816t_H
#define _cst816t_H
#include "gd32f4xx.h"

#define TOUCH_W 240
#define TOUCH_H 280

/*CST816T基本信息*/
extern uint8_t State;
extern uint16_t X_Axis;
extern uint16_t Y_Axis;
extern uint16_t g_TouchFlag;

/*IO操作函数*/ 
// PA6
// #define CST816T_RESET 						 PBout(4)
#define CST816T_RESET_RCU     RCU_GPIOC
#define CST816T_RESET_GPIO         GPIOC
#define CST816T_RESET_GPIO_PIN     GPIO_PIN_2
// PA7
// #define CST816T_IRQ 						 	 PBout(5)
#define CST816T_IRQ_RCU     	 RCU_GPIOC
#define CST816T_IRQ_GPIO         	 GPIOC
#define CST816T_IRQ_GPIO_PIN    	 GPIO_PIN_3
#define CST816T_EXTI_Line			 EXTI_3
/*SCL,SDA的位操作*/
// #define _CST816T_RESET_HIGH_() 		 (CST816T_RESET=1)
// #define _CST816T_RESET_LOW_() 		 (CST816T_RESET=0)
#define _CST816T_RESET_HIGH_()  gpio_bit_set(CST816T_RESET_GPIO,CST816T_RESET_GPIO_PIN)//RST  PC4
#define _CST816T_RESET_LOW_()  gpio_bit_reset(CST816T_RESET_GPIO,CST816T_RESET_GPIO_PIN)

#define CST816T_ADDRESS 0x2A

// 无触摸
#define CST816T_STATE_RELEASE 0xFF
// 检测到触摸
#define CST816T_STATE_PRESS_DOWN 0x00 
// 上滑
#define CST816T_STATE_SLIDE_UP 0x01
// 下滑
#define CST816T_STATE_SLIDE_DOWN 0x02
// 左滑
#define CST816T_STATE_SLIDE_LEFT 0x03 
// 右滑
#define CST816T_STATE_SLIDE_RIGHT 0x04 
// 单击
#define CST816T_STATE_SINGLE_CLICK 0x05
// 双击
#define CST816T_STATE_DOUBLE_CLICK 0x0B
// 长按
#define CST816T_STATE_LONG_PRESS 0x0C

/*
*Role     :
*         对于CST816T初始化
*/
void CST816T_Init(void);
/*
*Parameter:Addr
*					寄存器地址
*Parameter:Data,
*					存储地址
*Role     :
*         向CST816T对应寄存器发送数据
*/
void CST816T_SendByte(uint8_t Addr,uint8_t* Data);
/*
*Parameter:Addr
*					寄存器地址
*Parameter:Data,
*					存储地址
*Role     :
*         向CST816T对应寄存器接收数据
*/
void CST816T_ReceiveByte(uint8_t Addr,uint8_t* Data);
/*
*Role     :
*         得到手势状态，坐标
*/
void CST816_GetAction(void);
/*
*Role     :
*         对于CST816T复位
*/
void CST816T_Reset(void);
/*
*Role     :
*         清除上次坐标，状态信息
*/
void CST816T_ClrBaseDatas(void);

uint8_t CST816T_is_pressed(void);

void CST816T_get_xy(uint16_t* x,uint16_t* y);


#endif
