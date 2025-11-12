/*********************************************************************************************************************
* @file				wh_systick.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "wh_systick.h"

volatile static uint32_t delay;

////-------------------------------------------------------------------------------------------------------------------
//// @brief		systick的配置
//// @param		void			
//// @return		void
//// Sample usage:				无需用户调用，用户请使用h文件中的宏定义
////-------------------------------------------------------------------------------------------------------------------
//void systick_config(void)
//{
//    if(SysTick_Config(SystemCoreClock / 1000U))   //为1000Hz中断设置系统计时器
//		{
//        while(1)
//				{
//            ;
//				}
//     }
//    NVIC_SetPriority(SysTick_IRQn, 0x00U);        //配置时钟处理程序优先级
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick延时函数
// @param		time			需要延时的时间
// @return		void
// Sample usage:				无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void systick_delay (uint32_t time)
{
  SysTick->CTRL	= 0x00;                                   
	SysTick->LOAD	= time-1;                               //设置延时时间
	SysTick->VAL	= 0x00;                                 //清空计数器
	SysTick->CTRL	=	SysTick_CTRL_CLKSOURCE_Msk |					//时钟源选择 (core clk)
						SysTick_CTRL_ENABLE_Msk;						        //使能 systick
	while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		毫秒级systick延时函数
// @param		time			需要延时的时间（单位为毫秒）
// @return		void
// Sample usage:				systick_delay_ms(1000);   //延时1000毫秒
//-------------------------------------------------------------------------------------------------------------------
void delay_ms(uint32_t count)
{
    delay = count;

    while(0U != delay)
		{
			  ;
    }
}

////-------------------------------------------------------------------------------------------------------------------
//// @brief		延时减少函数
//// @param		void			
//// @return		void
//// Sample usage:				void delay_decrement(void)
////-------------------------------------------------------------------------------------------------------------------
//void delay_decrement(void)
//{
//    if(0U != delay)
//		{
//        delay--;
//    }
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick定时器
// @param		time			定时时间(0-0x00ffffff)
// @return		void
// Sample usage:				无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void systick_timing(uint32_t time)
{
  SysTick->CTRL	= 0x00;
	SysTick->LOAD = time-1;												// 设置延时时间
	SysTick->VAL = 0x00;												  // 清空计数器
	NVIC_SetPriority(SysTick_IRQn, 0x0);				  // 设置优先级
	SysTick->CTRL = ( 0 
		| SysTick_CTRL_ENABLE_Msk										// 使能 systick
		| SysTick_CTRL_TICKINT_Msk									// 使能中断
		| SysTick_CTRL_CLKSOURCE_Msk								// 时钟源选择 (core clk)
		);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick定时器启动
// @param		void
// @return		void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------
void systick_start (void)
{
	SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;		  // 设置延时时间
	SysTick->VAL = 0x00;												  // 清空计数器
	SysTick->CTRL = ( 0
		| SysTick_CTRL_ENABLE_Msk										// 使能 systick
		| SysTick_CTRL_CLKSOURCE_Msk								// 时钟源选择 (core clk)
		);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获得当前System tick timer的值
// @return		返回当前System tick timer的值
// Sample usage:				uint32 tim = systick_getval();
//-------------------------------------------------------------------------------------------------------------------
uint32_t systick_getval(void)
{
  return (SysTick_LOAD_RELOAD_Msk - SysTick->VAL);
}
