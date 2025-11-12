/*********************************************************************************************************************
* @file				wh_adc.h
* @author			Andreas WH
* @version			2.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#ifndef _WH_ADC_H
#define _WH_ADC_H

#include "gd32f4xx.h"

/* º¯Êý¶¨Òå */
void 			adc_init			(uint32_t ch_gpio, uint32_t ch_pin, uint32_t adc, rcu_periph_enum gpio_clk, rcu_periph_enum adc_clk);
uint16_t 		adc_convert			(uint32_t ch_gpio, uint32_t ch_pin);
uint16_t 		adc_mean_filter		(uint32_t ch_gpio, uint32_t ch_pin, uint8_t count);

#endif
