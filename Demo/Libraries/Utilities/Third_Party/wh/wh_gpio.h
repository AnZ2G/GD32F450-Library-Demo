/*********************************************************************************************************************
* @file				wh_gpio.h
* @author			Andreas WH
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#ifndef _WH_GPIO_H
#define _WH_GPIO_H

#include "gd32f4xx.h"

/* 函数定义 */

/* GPIO输出初始化 */
void gpio_output_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint8_t otype, uint32_t speed, uint32_t pin);

/* GPIO输入初始化 */
void gpio_input_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin);

/* GPIO复用功能设置 */
//void gpio_af_set(uint32_t gpio_periph, uint32_t alt_func_num, uint32_t pin);

/* GPIO引脚输出设置 */
void gpio_set_output(uint32_t gpio_periph, uint32_t pin, uint8_t dat);

/* GPIO引脚输出翻转 */
void gpio_toggle(uint32_t gpio_periph, uint32_t pin);

/* GPIO获取引脚输入 */
uint8_t gpio_get_input(uint32_t gpio_periph,uint32_t pin);

/* GPIO获取引脚输出 */
uint8_t gpio_get_output(uint32_t gpio_periph,uint32_t pin);

#endif
