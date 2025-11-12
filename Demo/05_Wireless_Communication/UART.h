/*********************************************************************************************************************
* @file				UART.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-27
********************************************************************************************************************/

#ifndef UART_H
#define UART_H

#include "gd32f4xx.h"
#include "stdio.h"
     

/* 板载串口定义 */
#define UARTn							2U

#define Wired_UART_TXD_PIN				GPIO_PIN_10
#define Wired_UART_RXD_PIN				GPIO_PIN_11
#define Wired_UART_GPIO_PORT			GPIOB
#define Wired_UART_GPIO_CLK				RCU_GPIOB
#define Wired_UART_CLK					RCU_USART2
#define Wired_UART_PERIPH				USART2
#define Wired_UART_nvic					USART2_IRQn
  
#define Wireless_UART_TXD_PIN			GPIO_PIN_5
#define Wireless_UART_RXD_PIN			GPIO_PIN_6
#define Wireless_UART_GPIO_PORT			GPIOD
#define Wireless_UART_GPIO_CLK			RCU_GPIOD
#define Wireless_UART_CLK				RCU_USART1
#define Wireless_UART_PERIPH			USART1
#define Wireless_UART_nvic				USART1_IRQn


/* 类型定义 */
typedef enum 
{
    Wired_UART = 0,
    Wireless_UART = 1
} uart_typedef_enum;


/* 函数定义 */
/* 初始化串口 */
void UART_Init(uart_typedef_enum uartnum, uint32_t baud);
/* 串口发送字节 */
void UART_Putchar(uint8_t data);
/* 串口发送数组 */
void UART_Putbuff(uint8_t *buff, uint32_t len);
/* 串口发送字符串 */
void UART_Putstr(const uint8_t *str);
/* 串口接收字节（while等待） */
void UART_Getchar(uint8_t *dat);
/* 串口接收字节（查询） */
uint8_t UART_Query(uint8_t *dat);
/* 串口中断使能 */
void UART_Interrupt_Enable(void);
/* 串口中断失能 */
void UART_Interrupt_Disable(void);


#endif
