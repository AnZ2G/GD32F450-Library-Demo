/*********************************************************************************************************************
* @file				pwm.h
* @author			Andreas WH
* @version			1.0
* @Target core		gd32F450ZIT6
* @date				2021-8-23
********************************************************************************************************************/

#ifndef _PWM_H
#define _PWM_H

#include <stdint.h>

void PWM_Config(uint16_t FrequencyDivision, uint16_t ReloadValue);
void PWM_Duty_Updata(uint8_t ch, uint16_t dutycycle);
void Steer_Updata(uint16_t period);

#endif 
