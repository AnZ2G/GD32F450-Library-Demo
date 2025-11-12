/*********************************************************************************************************************
* @file				wh_exti.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#ifndef WH_EXTI_H
#define WH_EXTI_H

#include "gd32f4xx.h"
#include "gd32f4xx_exti.h"

/* 类型定义 */
typedef enum 
{
    CHANGE = 0,
    RISING = 1,
    FALLING = 2
} exti_Mode_enum;


/* 函数定义 */

/* 使能外部中断 */
void exti_enable(uint32_t gpio_periph, uint32_t pin, exti_Mode_enum Mode);

/* 失能外部中断 */
void exti_disable(uint32_t pin);

#endif
