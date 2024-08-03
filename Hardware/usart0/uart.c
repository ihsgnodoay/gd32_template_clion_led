/*!
 * @file uart.c
 *
 * @brief 串口驱动
 *
 * @author GD32E50x
 * @version V1.0
*/
#include <stdio.h>
#include "uart.h"

/*!
 * @brief UART GPIO 初始化
 */
void uart_gpio_init_config(void)
{
	/* 配置时钟 */
	rcu_periph_clock_enable(UART_TX_PORT_RCU);
	rcu_periph_clock_enable(UART_RX_PORT_RCU);

	/* 配置模式 */
	gpio_mode_set(UART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_TX_PIN);
	gpio_mode_set(UART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_RX_PIN);

	/*配置复用模式*/
	gpio_af_set(UART_TX_PORT, GPIO_AF_7, UART_TX_PIN);
	gpio_af_set(UART_RX_PORT, GPIO_AF_7, UART_RX_PIN);

	/* 配置输出模式 */
	gpio_output_options_set(UART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART_TX_PIN);
	gpio_output_options_set(UART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART_RX_PIN);

}

/*!
 * @brief 初始化串口
 * 配置波特率
 */
void uart_config(void)
{
	uart_gpio_init_config();

	/* 使能串口时钟 */
	rcu_periph_clock_enable(RCU_USART0);

	/* USART复位 */
	usart_deinit(USART0);

	/* 配置串口波特率 */
	usart_baudrate_set(USART0, 115200U);

	/* 校验位*/
	usart_parity_config(USART0, USART_PM_NONE);

	/* 停止位 */
	usart_stop_bit_set(USART0, USART_STB_1BIT);

	/* 先送高位还是低位 */
	usart_data_first_config(USART0, USART_MSBF_LSB);

	/* 发送功能配置*/
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

#if USART0_RX_ENABLE
	/* 接收功能配置 */
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	/* 接收中断配置 */
	nvic_irq_enable(USART0_IRQn, 2, 2);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_interrupt_enable(USART0, USART_INT_IDLE);
#endif

	/* 使能串口 */
	usart_enable(USART0);
}


#if USART0_RX_ENABLE
/* 接收缓冲区 */
uint8_t get_recv_buffer[1024];
uint32_t g_recv_len = 0;

/*! \brief	串口中断服务函数，usart0中断名称 */
void USART0_IRQHandler(void)
{
	if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) == SET) {
		uint16_t value = usart_data_receive(USART0);
		get_recv_buffer[g_recv_len] = value;
		g_recv_len++;
	}

	if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
		/* 读取缓存区，清除缓存区 */
		usart_data_receive(USART0);
		get_recv_buffer[g_recv_len] = '\0';

		usart0_rec(get_recv_buffer, g_recv_len);
		g_recv_len = 0;
	}

}
#endif

/*!
 * @brief 发送一byte数据
 * @param data
 */
void send_byte(uint8_t data)
{
	usart_data_transmit(USART0, data);
	while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));

}

void send_string(uint8_t *str)
{
	while (str && *str) {
		send_byte(*str);
		str++;
	}
}

/*!
 * @brief 使用fputc函数实现printf打印输出
 * @param ch 一个字符一个字符的发，
 * @param f 固定格式
 * @return
 */
int fputc(int ch, FILE *f)
{
	send_byte((uint8_t) ch);
	return ch;
}

/*!
 *	@brief 接受数据
 */
void usart0_rec(uint8_t *data, uint32_t len)
{
	printf(">> %s\r\n", data);
}

