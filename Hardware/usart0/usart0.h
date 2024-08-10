/*
	通过串口发送消息给PC
	TX: PA9
	RX: PA10
*/

#ifndef UART_H__
#define UART_H__

#include "gd32f4xx.h"

/* TX */
#define UART_TX_PORT_RCU	RCU_GPIOA
#define UART_TX_PORT		GPIOA
#define UART_TX_PIN			GPIO_PIN_9

/* RX */
#define UART_RX_PORT_RCU	RCU_GPIOA
#define UART_RX_PORT		GPIOA
#define UART_RX_PIN			GPIO_PIN_10

#define USART0_RX_ENABLE	0

void uart_gpio_init_config(void);
void usart0_config(void);
void send_byte(uint8_t);
void send_string(uint8_t*);

#if USART0_RX_ENABLE
/* 接受 */
extern void usart0_rec(uint8_t* data, uint32_t len);
#endif

#endif