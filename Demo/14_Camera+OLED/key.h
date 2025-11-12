/*********************************************************************************************************************
* @file				KEY.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-26
********************************************************************************************************************/

#ifndef KEY_H
#define KEY_H

#include "gd32f4xx.h"
     

/* 板载按键定义 */
#define KEYn							6U

#define KEY_UP_PIN						GPIO_PIN_9
#define KEY_UP_GPIO_PORT				GPIOD
#define KEY_UP_GPIO_CLK					RCU_GPIOD

#define KEY_DOWN_PIN					GPIO_PIN_12
#define KEY_DOWN_GPIO_PORT				GPIOD
#define KEY_DOWN_GPIO_CLK				RCU_GPIOD

#define KEY_LEFT_PIN					GPIO_PIN_10
#define KEY_LEFT_GPIO_PORT				GPIOD
#define KEY_LEFT_GPIO_CLK				RCU_GPIOD

#define KEY_RIGHT_PIN					GPIO_PIN_11
#define KEY_RIGHT_GPIO_PORT				GPIOD
#define KEY_RIGHT_GPIO_CLK				RCU_GPIOD

#define KEY_SET_PIN						GPIO_PIN_8
#define KEY_SET_GPIO_PORT				GPIOD
#define KEY_SET_GPIO_CLK				RCU_GPIOD

#define KEY_BACK_PIN					GPIO_PIN_13
#define KEY_BACK_GPIO_PORT				GPIOD
#define KEY_BACK_GPIO_CLK				RCU_GPIOD

#define KEY_PRESS                    	0
#define KEY_RELEASE                    	1


/* 板载拨码开关定义 */
#define SWITCHn							4U

#define SWITCH1_PIN						GPIO_PIN_2
#define SWITCH1_GPIO_PORT				GPIOG
#define SWITCH1_GPIO_CLK				RCU_GPIOG

#define SWITCH2_PIN						GPIO_PIN_3
#define SWITCH2_GPIO_PORT				GPIOG
#define SWITCH2_GPIO_CLK				RCU_GPIOG

#define SWITCH3_PIN						GPIO_PIN_4
#define SWITCH3_GPIO_PORT				GPIOG
#define SWITCH3_GPIO_CLK				RCU_GPIOG

#define SWITCH4_PIN						GPIO_PIN_5
#define SWITCH4_GPIO_PORT				GPIOG
#define SWITCH4_GPIO_CLK				RCU_GPIOG

#define SWITCH_ON                    	0
#define SWITCH_OFF                    	1


typedef enum 
{
    KEY_UP = 0,
    KEY_DOWN = 1,
    KEY_LEFT = 2,
    KEY_RIGHT = 3,
    KEY_SET = 4,
    KEY_BACK = 5,
    KEY_ALL = 99
} key_typedef_enum;

typedef enum 
{
    SWITCH1 = 0,
    SWITCH2 = 1,
    SWITCH3 = 2,
    SWITCH4 = 3,
    SWITCH_ALL = 99,
} switch_typedef_enum;



/* 函数定义 */

/* 初始化按键 */
void KEY_Init(key_typedef_enum keynum);
/* 获取按键输入 */
uint8_t KEY_Get_Input(key_typedef_enum keynum);
/* 读取输入的按键值 */
uint8_t KEY_Read(uint8_t mode);
/* 初始化拨码开关 */
void SWITCH_Init(switch_typedef_enum switchnum);
/* 获取拨码开关输入 */
uint8_t SWITCH_Get_Input(switch_typedef_enum switchnum);
/* 使能按键中断 */
void KEY_EXTI_Enable(key_typedef_enum keynum);
/* 失能按键中断 */
void KEY_EXTI_Disanle(key_typedef_enum keynum);


#endif
