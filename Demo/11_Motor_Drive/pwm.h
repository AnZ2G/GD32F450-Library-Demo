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

#define FrequencyDivision 		10		//分频值
#define ReloadValue 			2000	//重装载值

//频率的计算：初始频率为200MHz，输出频率为 初始频率÷分频值÷重装载值
//若分频值为200，重装载值为1000，输出频率=200MHz/200/1000 = 1KHz

void pwm_config(void);
void pwm_duty_updata(uint8_t ch, uint16_t dutycycle);

#endif 
