/*********************************************************************************************************************
* @file				main.c
* @brief			电机驱动
* @author			HeFei WeiHuang
* @version			2.2
* @Target core		GD32F450ZIT6
* @date				2021-7-8
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何应用中间层库pwm配置
函数输出pwm信号，控制电机转动。

-在GD32F450学习板上，PA9，PA10配置为复用功能1，作为TIMER0通
道1,2。通过中间层库函数对预分频值、自动重装载值、输出比较值
进行设置。PA9引脚输出pwm信号，根据pwm占空比（输出比较值）对
电机转速进行控制。

-接通学习板电源和电机驱动板电源后，电机正转1S，停转1S，反转1S，停转1S。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "headfile.h"
#include "systick.h"
#include <stdio.h>

void timer_config(void);

int main(void)
{

    systick_config();

    /* 配置GPIO时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOD);

    /* 配置GPIO，PA9为复用功能1，作为TIMER0通道1,PA10为复用功能1，作为TIMER0通道2 */
    gpio_output_init(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_init(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
//    gpio_output_init(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
//    gpio_output_init(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);
    gpio_af_set(GPIOE, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    /* 配置TIMER0通道1PWM输出，预分频值为199，自动重装载值为9999，输出比较值为2099.电机正转1s */

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,0);
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);	


    timer_config();

    while (1)
    {
        timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,1999);						//配置外设定时器的通道输出比较值
        timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);
        delay_1ms(3000);	
        timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);						//配置外设定时器的通道输出比较值
        timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,1999);
        delay_1ms(3000);	
         

    }
}


void timer_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;											//通道输出极性配置为高																							//定时器时钟使能
    timer_deinit(TIMER0);																		//复位外设定时器

    timer_initpara.prescaler         = 199;													//写入预分频值
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;											//对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;											//计数方式为向上计数
    timer_initpara.period            = 19999;													//写入自动重装载值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;											//选择时钟分频因子
    timer_initpara.repetitioncounter = 0;															//写入重复计数值
    timer_init(TIMER0,&timer_initpara);														//初始化外设定时器

    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;											//通道输出极性配置为高
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;												//通道输出使能
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;											//互补通道输出极性配置为高
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;												//互补通道输出禁能
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;											//空闲状态下通道输出为低
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;										//空闲状态下互补通道输出为低

    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);								//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);						//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);				//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);								//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);						//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);				//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);								//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);						//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);				//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);								//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,0);						//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);						//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);				//定时器通道输出影子寄存器禁能

    timer_primary_output_config(TIMER0,ENABLE);												//定时器通道输出使能
    timer_auto_reload_shadow_enable(TIMER0);													//定时器自动重载影子使能
    timer_enable(TIMER0);	
}
