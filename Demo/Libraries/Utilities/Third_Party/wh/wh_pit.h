/*********************************************************************************************************************
* @file				wh_pit.h
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#ifndef _wh_pit_h
#define _wh_pit_h

#include "gd32f4xx.h"

void timer_interrupt_init(uint32_t timer_periph, uint16_t timer_period);

#endif
