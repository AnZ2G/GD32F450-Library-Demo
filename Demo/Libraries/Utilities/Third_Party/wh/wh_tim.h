/*********************************************************************************************************************
* @file				wh_timer.h
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#ifndef _WH_TIMER_H
#define _WH_TIMER_H

#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_timer.h"

static void tim_encoder_pin_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t speed, uint8_t otype, uint32_t alt_func_num, uint32_t pin);
void tim_encoder_init(uint32_t timer_periph, uint32_t PULSE_A_gpio_periph, uint32_t PULSE_A_speed, uint32_t PULSE_A_pin,
                      uint32_t PULSE_B_gpio_periph, uint32_t PULSE_B_speed, uint32_t PULSE_B_pin, uint16_t channel,
                      rcu_periph_enum periph, uint32_t mode, uint32_t pull_up_down, uint8_t otype, uint32_t alt_func_num);
void tim_encoder_rst(uint32_t timer_periph);
int16_t tim_encoder_get_count(uint32_t timer_periph);
void timer_struct_config(timer_parameter_struct *timer_initpara, uint16_t freq_div, uint16_t period_temp);



#endif /* _WH_TIMER_H */
