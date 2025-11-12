/*********************************************************************************************************************
* @file				main.c
* @brief			定时器中断
* @author			HeFei WeiHuang
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何配置LED的GPIO，
并通过定时器中断控制LED亮灭。

-在GD32F450学习板上，PD0连接LED2。

-LED2闪烁，频率为1秒。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "gd32f4xx_it.h"
#include "headfile.h"
#include "systick.h"
#include <stdio.h>

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 配置GPIO时钟 */
    rcu_periph_clock_enable(RCU_GPIOD);

    /* 配置GPIO */
    gpio_output_init(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    timer_interrupt_init(TIMER6, 10000);
    while(1)
    {
        ;
    }
}
