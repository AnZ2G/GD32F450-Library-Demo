/*********************************************************************************************************************
* @file				wh_uart.h
* @author			Andreas WH
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#ifndef _WH_UART_H
#define _WH_UART_H

#include "gd32f4xx.h"


/* 函数定义 */

/* UART TXD引脚初始化 */
void uart_tx_init(uint32_t gpio, uint32_t pin, rcu_periph_enum clk);

/* UART RXD引脚初始化 */
void uart_rx_init(uint32_t gpio, uint32_t pin, rcu_periph_enum clk);

/* UART 外设初始化 */
void uart_init(rcu_periph_enum uart_clk, uint32_t uart_periph, uint32_t baud);

/* UART 发送字节 */
void uart_putchar(uint32_t uart_periph, uint8_t data);

/* UART 发送数组 */
void uart_putbuff(uint32_t uart_periph, uint8_t *buff, uint32_t len);

/* UART 发送字符串 */
void uart_putstr(uint32_t uart_periph, const uint8_t *str);

/* UART 接收字节（whlie等待） */
void uart_getchar(uint32_t uart_periph, uint8_t *dat);

/* UART 接收字节（查询接收） */
uint8_t uart_query(uint32_t uart_periph, uint8_t *dat);

/* UART 中断初始化 */
void uart_interrupt_init(uint32_t uart_periph, usart_interrupt_enum int_flag, uint32_t status, uint32_t uart_irq);

/* UART 获取中断标志位 */
uint8_t uart_interrupt_flag_get(uint32_t uart_periph, usart_interrupt_enum int_flag);

/* UART 清除中断标志位 */
void uart_interrupt_flag_clear(uint32_t uart_periph, usart_interrupt_flag_enum int_flag);

#endif
