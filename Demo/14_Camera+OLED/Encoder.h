/*********************************************************************************************************************
* @file				Encoder.h
* @author			Andreas WH
* @version			1.0
* @Target core		gd32F450ZIT6
* @date				2021-8-23
********************************************************************************************************************/

#ifndef _ENCODER_H
#define _ENCODER_H

#include <stdint.h>



/* 板载编码器接口定义 */
#define Encodern							2U
#define EncoderModen						2U

#define Encoder1_PIN_A						GPIO_PIN_5
#define Encoder1_PIN_B						GPIO_PIN_4
#define Encoder1_GPIO_PORT					GPIOB
#define Encoder1_GPIO_CLK					RCU_GPIOB
#define Encoder1_GPIO_AF					GPIO_AF_2
#define Encoder1_TIMER						TIMER2
#define Encoder1_TIMER_CLK					RCU_TIMER2
#define Encoder1_CHANNAL_A					TIMER_CH_1
#define Encoder1_INT_CHANNAL				TIMER_INT_CH1
#define Encoder1_INT_FLAG					TIMER_INT_FLAG_CH1
#define Encoder1_TIMER_NVIC					TIMER2_IRQn
  
#define Encoder2_PIN_A						GPIO_PIN_1
#define Encoder2_PIN_B						GPIO_PIN_0
#define Encoder2_GPIO_PORT					GPIOA
#define Encoder2_GPIO_CLK					RCU_GPIOA
#define Encoder2_GPIO_AF					GPIO_AF_2
#define Encoder2_TIMER						TIMER4
#define Encoder2_TIMER_CLK					RCU_TIMER4
#define Encoder2_CHANNAL_A					TIMER_CH_1
#define Encoder2_INT_CHANNAL				TIMER_INT_CH1
#define Encoder2_INT_FLAG					TIMER_INT_FLAG_CH1
#define Encoder2_TIMER_NVIC					TIMER4_IRQn

#define CollectionTime 			5000 //编码器采集时间（us）

/* 类型定义 */
typedef enum 
{
    Encoder1 = 0,
    Encoder2 = 1,
    Encoder_ALL = 99
} encoder_typedef_enum;

typedef enum 
{
    Mode1 = 0,		//带方向编码器
    Mode2 = 1		//正交编码器
} encodermode_typedef_enum;

void Encoder_Init(encoder_typedef_enum Encodernum, encodermode_typedef_enum mode);
/* 读取编码器数值 */
int16_t Encoder_Read(encoder_typedef_enum Encodernum);

void TIMER2_IRQHandler(void);

#endif /* SYS_TICK_H */
