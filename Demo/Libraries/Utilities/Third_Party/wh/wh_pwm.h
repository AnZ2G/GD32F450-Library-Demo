/*********************************************************************************************************************
* @file				wh_pwm.h
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#ifndef _WH_pwm_h
#define _WH_pwm_h

#include "gd32f4xx.h"

#define PWM_DUTY_MAX		50000
void pwm_pin_init(rcu_periph_enum periph, uint32_t gpio_periph, uint32_t speed, uint32_t pin, uint32_t alt_func_num);											//PWM 引脚初始化 内部调用
void pwm_init (rcu_periph_enum periph, uint32_t timer_periph, uint16_t channel,uint16_t freq_div, uint16_t period_temp, uint16_t match_temp);

#endif
