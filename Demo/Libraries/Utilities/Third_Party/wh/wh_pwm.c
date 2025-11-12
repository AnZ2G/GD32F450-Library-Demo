/*********************************************************************************************************************
* @file				wh_pwm.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#include <stdio.h>
#include "systick.h"
#include "wh_pwm.h"
#include "wh_tim.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM 引脚初始化 内部调用
// @param		periph: 设置GPIO时钟（与所选GPIO保持一致）
            //RCU_GPIOx (x=A,B,C,D,E,F,G): GPIO ports clock
// @param		gpio_periph:  GPIO初始化
						//GPIOx(x = A,B,C,D,E,F,G)
// @param   speed: 设置GPIO输出最大速度值
            //only one parameter can be selected which is shown as below:
            //GPIO_OSPEED_10MHZ: output max speed 10MHz
            //GPIO_OSPEED_2MHZ: output max speed 2MHz
            //GPIO_OSPEED_50MHZ: output max speed 50MHz
            //GPIO_OSPEED_MAX: output max speed more than 50MHz
// @param   pin: 设置GPIO引脚
            //one or more parameters can be selected which are shown as below:
            //GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @param   alt_func_num: 配置GPIO备用功能（与所选TIMER保持一致）
            //GPIO_AF_1: TIMER0, TIMER1
            //GPIO_AF_2: TIMER2, TIMER3, TIMER4
            //GPIO_AF_3: TIMER7, TIMER8, TIMER9, TIMER10
// @return		void			NULL
// Sample usage:				pwm_pin_init(RCU_GPIOA, GPIOA, GPIO_OSPEED_50MHZ, GPIO_PIN_8)；
//-------------------------------------------------------------------------------------------------------------------
void pwm_pin_init(rcu_periph_enum periph, uint32_t gpio_periph, uint32_t speed, uint32_t pin, uint32_t alt_func_num)
{
    rcu_periph_clock_enable(periph);    //GPIO时钟使能
	
    gpio_mode_set(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, speed, pin);
    gpio_af_set(gpio_periph, alt_func_num, pin);    //GPIO初始化
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM 初始化
// @param		periph: 		定时器时钟
// 	@arg		RCU_TIMERx (x=0,1,2,3,4,5,6,7,8,9,10,11,12,13,TIMER8..13 are not available for EPRT series): TIMER clock
// @param		timer_periph: 	定时器外设号
// @param		channel:  		通道
// 	@arg		TIMER_CH_0: TIMER channel0(TIMERx(x=0..4,7..13))
// 	@arg		TIMER_CH_1: TIMER channel1(TIMERx(x=0..4,7,8,11))
// 	@arg		TIMER_CH_2: TIMER channel2(TIMERx(x=0..4,7))
// 	@arg		TIMER_CH_3: TIMER channel3(TIMERx(x=0..4,7))
// @param		freq_div		预分频值
// @param		period_temp		自动重装载值
// @param		match_temp		通道输出比较值
// @return		void			NULL
// Sample usage:				pwm_init(RCU_TIMER4, TIMER4, TIMER_CH_3, 199, 19999, 1599);
//TIMER4主频200M，频率=主频/预分频/自动重装载，占空比=输出比较值/自动重装载值。
//如199，19999，1599.频率=200M/200/20000 = 50HZ,占空比 = 1600/20000 = 8％。
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(rcu_periph_enum periph, uint32_t timer_periph, uint16_t channel, uint16_t freq_div, uint16_t period_temp, uint16_t match_temp)
{
    
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(periph);
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;											//通道输出极性配置为高																							//定时器时钟使能
    timer_deinit(timer_periph);																		//复位外设定时器

    timer_initpara.prescaler         = freq_div;													//写入预分频值
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;											//对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;											//计数方式为向上计数
    timer_initpara.period            = period_temp;													//写入自动重装载值
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;											//选择时钟分频因子
    timer_initpara.repetitioncounter = 0;															//写入重复计数值
    timer_init(timer_periph,&timer_initpara);														//初始化外设定时器

    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;											//通道输出极性配置为高
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;												//通道输出使能
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;											//互补通道输出极性配置为高
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;												//互补通道输出禁能
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;											//空闲状态下通道输出为低
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;										//空闲状态下互补通道输出为低

    timer_channel_output_config(timer_periph,channel,&timer_ocintpara);								//外设定时器的通道输出配置
    timer_channel_output_pulse_value_config(timer_periph,channel,match_temp);						//配置外设定时器的通道输出比较值
    timer_channel_output_mode_config(timer_periph,channel,TIMER_OC_MODE_PWM0);						//配置外设定时器通道输出比较模式（PWM0/PWM1）
    timer_channel_output_shadow_config(timer_periph,channel,TIMER_OC_SHADOW_DISABLE);				//定时器通道输出影子寄存器禁能
    timer_primary_output_config(timer_periph,ENABLE);												//定时器通道输出使能
    timer_auto_reload_shadow_enable(timer_periph);													//定时器自动重载影子使能
    timer_enable(timer_periph);																		//定时器使能
}

