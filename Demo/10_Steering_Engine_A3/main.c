/*********************************************************************************************************************
* @file				main.c
* @brief			舵机
* @author			HeFei WeiHuang
* @version			2.1
* @Target core		GD32F450ZIT6
* @date				2021-4-8
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何应用中间层库pwm配置
函数输出pwm信号，对舵机角度进行控制。

-在GD32F450学习板上，PA3配置为复用功能2，作为TIMER4通
道3。通过中间层库函数对预分频值、自动重装载值、输出比较值
进行设置。PA3引脚输出pwm信号。根据pwm占空比（输出
比较值）对舵机转动角度进行控制。

-接通电源后，舵机来回往复转动。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include "headfile.h"
#include <stdio.h>
int i = 0;

int main(void)
{

    systick_config();

    /* 配置GPIO时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* 配置PA3引脚及复用功能，作为TIMER4通道3 */
    gpio_output_init(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_3);

    /* 配置TIMER4通道3PWM输出，预分频值为199，自动重装载值为19999，输出比较值为1599 */
    pwm_init(RCU_TIMER4, TIMER4, TIMER_CH_3, 99, 19999, 1199);

    while (1)
    {
//        /* 设置TIMER4通道3PWM输出比较值为1199 */
//       timer_channel_output_pulse_value_config(TIMER4,TIMER_CH_3,1199);
       delay_1ms(2000);

//        /* 设置TIMER4通道3PWM输出比较值为1799 */
//        timer_channel_output_pulse_value_config(TIMER4,TIMER_CH_3,1799);
//       delay_1ms(2000);
    }
}
