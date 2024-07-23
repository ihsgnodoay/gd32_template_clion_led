/*!
 * @file uart.c
 *
 * @brief ��������
 *
 * @author GD32E50x
 * @version V1.0
*/
#include <stdio.h>
#include "uart.h"

/*!
 * @brief UART GPIO ��ʼ��
 */
void uart_gpio_init_config(void)
{
	/* ����ʱ�� */
	rcu_periph_clock_enable(UART_TX_PORT_RCU);
	rcu_periph_clock_enable(UART_RX_PORT_RCU);

	/* ����ģʽ */
	gpio_mode_set(UART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_TX_PIN);
	gpio_mode_set(UART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, UART_RX_PIN);

	/*���ø���ģʽ*/
	gpio_af_set(UART_TX_PORT, GPIO_AF_7, UART_TX_PIN);
	gpio_af_set(UART_RX_PORT, GPIO_AF_7, UART_RX_PIN);

	/* �������ģʽ */
	gpio_output_options_set(UART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART_TX_PIN);
	gpio_output_options_set(UART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, UART_RX_PIN);

}

/*!
 * @brief ��ʼ������
 * ���ò�����
 */
void uart_config(void)
{

	/* ʹ�ܴ���ʱ�� */
	rcu_periph_clock_enable(RCU_USART0);

	/* USART��λ */
	usart_deinit(USART0);

	/* ���ô��ڲ����� */
	usart_baudrate_set(USART0, 115200U);

	/* У��λ*/
	usart_parity_config(USART0, USART_PM_NONE);

	/* ֹͣλ */
	usart_stop_bit_set(USART0, USART_STB_1BIT);

	/* ���͸�λ���ǵ�λ */
	usart_data_first_config(USART0, USART_MSBF_LSB);

	/* ���͹�������*/
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

#if USART0_RX_ENABLE
	/* ���չ������� */
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	/* �����ж����� */
	nvic_irq_enable(USART0_IRQn, 2, 2);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_interrupt_enable(USART0, USART_INT_IDLE);
#endif

	/* ʹ�ܴ��� */
	usart_enable(USART0);
}


#if USART0_RX_ENABLE
/* ���ջ����� */
uint8_t get_recv_buffer[1024];
uint32_t g_recv_len = 0;

/*! \brief	�����жϷ�������usart0�ж����� */
void USART0_IRQHandler(void)
{
	if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) == SET) {
		uint16_t value = usart_data_receive(USART0);
		get_recv_buffer[g_recv_len] = value;
		g_recv_len++;
	}

	if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
		/* ��ȡ����������������� */
		usart_data_receive(USART0);
		get_recv_buffer[g_recv_len] = '\0';

		usart0_recv(get_recv_buffer, g_recv_len);
		g_recv_len = 0;
	}

}
#endif

/*!
 * @brief ����һbyte����
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
 * @brief ʹ��fputc����ʵ��printf��ӡ���
 * @param ch һ���ַ�һ���ַ��ķ���
 * @param f �̶���ʽ
 * @return
 */
int fputc(int ch, FILE *f)
{
	send_byte((uint8_t)ch);
	return ch;
}
