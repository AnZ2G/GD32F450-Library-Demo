/*********************************************************************************************************************
* @file				pwm.c
* @author			Andreas WH
* @version			1.0
* @Target core		gd32F450ZIT6
* @date				2021-8-23
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "headfile.h"
#include <stdio.h>
#include "pwm.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置PWM
// @return		void
// Sample usage:				encoder_config();
//-------------------------------------------------------------------------------------------------------------------
void pwm_config(void)
{
    /* 配置pwm引脚及复用功能 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOE);

	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);
    gpio_af_set(GPIOE, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);

    /* 配置定时器结构体 */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER0);											//定时器时钟使能
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;							//通道输出极性配置为高	
    timer_deinit(TIMER0);															//复位外设定时器

    timer_initpara.prescaler         = FrequencyDivision-1;							//写入预分频值
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;							//对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;							//计数方式为向上计数
    timer_initpara.period            = ReloadValue;									//写入自动重装载值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;							//选择时钟分频因子
    timer_initpara.repetitioncounter = 0;											//写入重复计数值
    timer_init(TIMER0,&timer_initpara);												//初始化外设定时器

    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;							//通道输出极性配置为高
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;								//通道输出使能
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;							//互补通道输出极性配置为高
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;								//互补通道输出禁能
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;							//空闲状态下通道输出为低
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;						//空闲状态下互补通道输出为低

    /* 配置定时器通道 */
    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);				//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);					//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);			//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);	//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);				//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,0);					//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);			//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);	//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);				//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);					//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);			//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);	//定时器通道输出影子寄存器禁能

    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);				//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,0);					//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);			//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);	//定时器通道输出影子寄存器禁能

    timer_primary_output_config(TIMER0,ENABLE);										//定时器通道输出使能
    timer_auto_reload_shadow_enable(TIMER0);										//定时器自动重载影子使能
    timer_enable(TIMER0);															//使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		更新PWM占空比
// @return		void
// Sample usage:				encoder_config();
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty_updata(uint8_t ch, uint16_t dutycycle)
{
    uint16_t output_value,channal;
    output_value = dutycycle*ReloadValue/1000;
    if(ch==0) channal = TIMER_CH_0;
    if(ch==1) channal = TIMER_CH_1;
    if(ch==2) channal = TIMER_CH_2;
    if(ch==3) channal = TIMER_CH_3;
    timer_channel_output_pulse_value_config(TIMER0,channal,output_value-1);
}
