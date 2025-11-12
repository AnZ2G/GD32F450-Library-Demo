/*********************************************************************************************************************
* @file				wh_systick.h
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-2
********************************************************************************************************************/

#ifndef _wh_systick_h
#define _wh_systick_h

#include <stdint.h>

#define DELAY_CLK SystemCoreClock

#define USEC_TO_COUNT(us, clockFreqInHz)		(uint64_t)(((uint64_t)(us) * (clockFreqInHz)) / 1000000U)
#define COUNT_TO_USEC(count, clockFreqInHz)		(uint64_t)((uint64_t)count * 1000000U / clockFreqInHz)

#define MSEC_TO_COUNT(ms, clockFreqInHz)		(uint64_t)((uint64_t)ms * clockFreqInHz / 1000U)
#define COUNT_TO_MSEC(count, clockFreqInHz)		(uint64_t)((uint64_t)count * 1000U / clockFreqInHz)

/* 滴答时钟的配置 */
//void systick_config(void);

//------------------------------------以下宏定义用于延时------------------------------------
/* 时钟延时函数 */
void systick_delay (uint32_t time);
/* 时钟延时毫秒级函数 */
void delay_ms(uint32_t count);                     // 毫秒级延时函数 由于滴答定时器最大只有24位，因此用函数实现，这样延时时间范围较宽
#define systick_delay_us(time)		systick_delay(USEC_TO_COUNT(time, DELAY_CLK))		    // 设置延时时间 单位us 范围0 - 16777215(0xffffff)/(DELAY_CLK*1000)ms	主频96M systick最大延时时间大约为33ms
#define systick_delay_ns(time)		systick_delay(USEC_TO_COUNT(time, DELAY_CLK/1000))	// 设置延时时间 单位ns 范围0 - 16777215(0xffffff)*1000/(DELAY_CLK)ns	主频96M systick最大延时时间大约为33ms
/* 逐渐将延时减少 */
//void delay_decrement(void);

//------------------------------------以下宏定义用于定时------------------------------------  
void systick_timing(uint32_t time);
#define systick_timing_ms(time)		systick_timing(MSEC_TO_COUNT(time, DELAY_CLK))		  // 设置定时时间 单位ms 范围0 - 16777215(0xffffff)/(DELAY_CLK*1000)ms	主频96M 最大定时时间大约为33ms
#define systick_timing_us(time)		systick_timing(USEC_TO_COUNT(time, DELAY_CLK))		  // 设置定时时间 单位us 范围0 - 16777215(0xffffff)/(DELAY_CLK)us			主频96M 最大定时时间大约为33ms
#define systick_timing_ns(time)		systick_timing(USEC_TO_COUNT(time, DELAY_CLK/1000))	// 设置定时时间 单位ns 范围0 - 16777215(0xffffff)*1000/(DELAY_CLK)ns	主频96M 最大定时时间大约为33ms

//------------------------------------以下宏定义用于获取当前时间------------------------------------
void systick_start (void);
uint32_t systick_getval(void);
#define systick_getval_ms()			COUNT_TO_MSEC(systick_getval(),DELAY_CLK)			   // 获取当前计时时间 单位ms
#define systick_getval_us()			COUNT_TO_USEC(systick_getval(),DELAY_CLK)			   // 获取当前计时时间 单位us
#define systick_getval_ns()			COUNT_TO_USEC(systick_getval()*1000,DELAY_CLK)	 // 获取当前计时时间 单位ns


#endif
