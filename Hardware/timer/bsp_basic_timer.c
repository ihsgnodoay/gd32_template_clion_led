///********************************************************************************
//  * 文 件 名: bsp_basic_timer.c
//  * 版 本 号: 初版
//  * 修改作者: LC
//  * 修改日期: 2022年04月18日
//  * 功能介绍:
//  ******************************************************************************
//  * 注意事项:
//*********************************************************************************/
//
//#include "bsp_basic_timer.h"
////#include "bsp_led.h"
//#include "stdio.h"
//#include "lvgl.h"
///************************************************
//函数名称 ： basic_timer_config
//功    能 ： 基本定时器配置
//参    数 ： pre：时钟预分频值
//					  per：周期
//返 回 值 ： 无
//作    者 ： LC
//*************************************************/
//void basic_timer_config(uint16_t pre,uint16_t per)
//{
//	/* 一个周期的时间T = 1/f, 定时时间time = T * 周期
//    设预分频值位pre,周期位per
//    time = (pre + 1) * (per + 1) / psc_clk
//	*/
//	timer_parameter_struct timere_initpara; 							// 定义定时器结构体
//	/* 开启时钟 */
//	rcu_periph_clock_enable(BSP_TIMER_RCU); 							// 开启定时器时钟
//	/* CK_TIMERx = 4 x CK_APB1  = 4x50M = 200MHZ */
//	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4); // 配置定时器时钟
//
//
//	timer_deinit(BSP_TIMER);														  // 复位定时器
//	/* 配置定时器参数 */
//	timere_initpara.prescaler = pre-1;                    //  时钟预分频值 0-65535   psc_clk = CK_TIMER / pre
//	timere_initpara.alignedmode = TIMER_COUNTER_EDGE;     // 边缘对齐
//	timere_initpara.counterdirection = TIMER_COUNTER_UP;  // 向上计数
//	timere_initpara.period = per-1;                       // 周期
//	/* 在输入捕获的时候使用  数字滤波器使用的采样频率之间的分频比例 */
//	timere_initpara.clockdivision = TIMER_CKDIV_DIV1;     // 分频因子
//  /* 只有高级定时器才有 配置为x，就重复x+1次进入中断 */
//	timere_initpara.repetitioncounter = 0;							  // 重复计数器 0-255
//
//	timer_init(BSP_TIMER,&timere_initpara);								// 初始化定时器
//
//	/* 配置中断优先级 */
//	nvic_irq_enable(BSP_TIMER_IRQ,3,2); 									// 设置中断优先级为 3,2
//	/* 使能中断 */
//	timer_interrupt_enable(BSP_TIMER,TIMER_INT_UP);       // 使能更新事件中断
//	/* 使能定时器 */
//	timer_enable(BSP_TIMER);
//}
//
///************************************************
//函数名称 ： BSP_TIMER_IRQHandler
//功    能 ： 基本定时器中断服务函数
//参    数 ： 无
//返 回 值 ： 无
//作    者 ： LC
//*************************************************/
//void BSP_TIMER_IRQHANDLER(void)
//{
//	/* 这里是定时器中断 */
//  if(timer_interrupt_flag_get(BSP_TIMER,TIMER_INT_FLAG_UP) == SET)
//	{
//		timer_interrupt_flag_clear(BSP_TIMER,TIMER_INT_FLAG_UP);  // 清除中断标志位
//		/* 执行功能 */
//		lv_tick_inc(1);
//	}
//}
