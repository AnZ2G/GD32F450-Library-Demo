/*********************************************************************************************************************
* @file				wh_pit.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#include "wh_pit.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		定时器中断初始化
// @param		timer_periph	选择中断使用的定时器（TIMER1-10）
// @param		timer_period	设置中断时间（ms）
// @return		void
// Sample usage:				timer_interrupt_init(TIMER2, 1000); 开启TIMER2 1ms定时器中断
//-------------------------------------------------------------------------------------------------------------------
void timer_interrupt_init(uint32_t timer_periph, uint16_t timer_period)
{
    if(timer_periph == TIMER1)
    {
        nvic_irq_enable(TIMER1_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER1);
    }
    else if(timer_periph == TIMER2)
    {
        nvic_irq_enable(TIMER2_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER2);
    }
    else if(timer_periph == TIMER3)
    {
        nvic_irq_enable(TIMER3_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER3);
    }
    else if(timer_periph == TIMER4)
    {
        nvic_irq_enable(TIMER4_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER4);
    }
    else if(timer_periph == TIMER5)
    {
        rcu_periph_clock_enable(RCU_TIMER5);
    }
    else if(timer_periph == TIMER6)
    {
        nvic_irq_enable(TIMER6_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER6);
    }
    else if(timer_periph == TIMER7)
    {
        rcu_periph_clock_enable(RCU_TIMER7);
    }
    else if(timer_periph == TIMER8)
    {
		nvic_irq_enable(TIMER0_BRK_TIMER8_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER8);
    }
    else if(timer_periph == TIMER9)
    {
		nvic_irq_enable(TIMER0_UP_TIMER9_IRQn, 0, 0);
        rcu_periph_clock_enable(RCU_TIMER9);
    }
    else if(timer_periph == TIMER10)
    {
		nvic_irq_enable(TIMER0_TRG_CMT_TIMER10_IRQn,1,1);
        rcu_periph_clock_enable(RCU_TIMER10);
    }

    timer_parameter_struct timer_initpara;
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /* 复位外设TIMER */
    timer_deinit(timer_periph);
    /* 将TIMER初始化参数结构体中所有参数初始化为默认值 */
    timer_struct_para_init(&timer_initpara);
    /* TIMER的配置 */
    timer_initpara.prescaler         = 199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = timer_period  - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(timer_periph, &timer_initpara);

    /* 清除外设TIMER的更新中断标志位 */
    timer_interrupt_flag_clear(timer_periph, TIMER_INT_FLAG_UP);
    /* 使能TIMER中断 */
    timer_interrupt_enable(timer_periph, TIMER_INT_UP);
    /* 使能中断更新并使能TIMER */
    timer_enable(timer_periph);	
}
