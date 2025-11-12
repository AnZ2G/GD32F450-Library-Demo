/*********************************************************************************************************************
* @file				Encoder.c
* @author			Andreas WH
* @version			1.0
* @Target core		gd32F450ZIT6
* @date				2021-8-23
********************************************************************************************************************/

#include "gd32f4xx.h"
#include <stdio.h>
#include "Encoder.h"

uint16_t temp = 0;
int16_t value = 0;

//-------------------------------------------------------------------------------------------------------------------
// @brief		编码器定时器配置
// @return		void
// Sample usage:				encoder_config();
//-------------------------------------------------------------------------------------------------------------------
void encoder_config(void)
{
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* 配置PB5引脚及复用功能，作为TIMER2通道 */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_5);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_5);
	/* 配置PB0引脚，读取方向 */
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);

    /* 配置定时器时钟 */
    rcu_periph_clock_enable(RCU_TIMER2);

    /* 配置定时器时钟预分频器 */
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    /* 初始化TIMER2 */
    timer_deinit(TIMER2);

    /* 配置定时器结构体并配置定时器TIMER2 */
    timer_parameter_struct timer_initpara;
    timer_initpara.prescaler         = 199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = CollectionTime - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

  	timer_ic_parameter_struct timer_icinitpara;
   	timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
  	timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
  	timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter = 0;
    timer_input_capture_config(TIMER2, TIMER_CH_1, &timer_icinitpara);

    /* 使能定时器重装载 */
    timer_auto_reload_shadow_enable(TIMER2);

    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_CH1);
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    timer_interrupt_enable(TIMER2, TIMER_INT_CH1);

    nvic_irq_enable(TIMER2_IRQn, 0, 0);
    /* 使能定时器TIMER2 */
    timer_enable(TIMER2);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取编码器值
// @return		int16
// Sample usage:				int16 value;value = encoder_config();
//-------------------------------------------------------------------------------------------------------------------
int16_t GetEncoderValue(void)
{
    return value;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		定时器中断函数（内部函数）
// @return		void
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void TIMER2_IRQHandler(void)
{
    if (RESET != timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_CH1)) 
    {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_CH1);
        temp++;
    }
    else if (RESET != timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
        value = temp;
        temp = 0;
        if(gpio_input_bit_get(GPIOB, GPIO_PIN_0)) value = -value;
	}
}
