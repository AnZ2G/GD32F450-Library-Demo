/*********************************************************************************************************************
* @file				LED.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-26
********************************************************************************************************************/

#ifndef LED_H
#define LED_H

#include "gd32f4xx.h"
     

/* 板载LED定义 */
#define LEDn                             4U

#define LED1_PIN						GPIO_PIN_0
#define LED1_GPIO_PORT					GPIOD
#define LED1_GPIO_CLK					RCU_GPIOD
  
#define LED2_PIN						GPIO_PIN_1
#define LED2_GPIO_PORT					GPIOD
#define LED2_GPIO_CLK					RCU_GPIOD
  
#define LED3_PIN						GPIO_PIN_2
#define LED3_GPIO_PORT					GPIOD
#define LED3_GPIO_CLK					RCU_GPIOD

#define LED4_PIN						GPIO_PIN_3
#define LED4_GPIO_PORT					GPIOD
#define LED4_GPIO_CLK					RCU_GPIOD

#define LED_ON                    		0
#define LED_OFF                    		1


/* 类型定义 */
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3,
    LED_ALL = 99
} led_typedef_enum;


/* 函数定义 */

/* 初始化LED */
void LED_Init(led_typedef_enum lednum);
/* 点亮LED */
void LED_On(led_typedef_enum lednum);
/* 熄灭LED */
void LED_Off(led_typedef_enum lednum);
/* 切换LED状态 */
void LED_Toggle(led_typedef_enum lednum);


#endif
