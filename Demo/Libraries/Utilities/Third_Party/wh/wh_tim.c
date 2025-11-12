/*********************************************************************************************************************
* @file				tim.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-2
********************************************************************************************************************/

#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_timer.h"
#include "wh_tim.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		encoder 引脚初始化 内部调用
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G,H,I)
// @param		mode				GPIO的模式，只能选择以下参数：
// 	@arg		GPIO_MODE_INPUT:	输入模式
// 	@arg		GPIO_MODE_OUTPUT:	输出模式
// 	@arg		GPIO_MODE_AF:		复用模式
// 	@arg		GPIO_MODE_ANALOG:	模拟模式
// @param		pull_up_down		引脚使用上拉电阻还是下拉电阻，只能选择以下参数：
// 	@arg		GPIO_PUPD_NONE:		浮空模式，不使用电阻
// 	@arg		GPIO_PUPD_PULLUP:	使用上拉电阻	
// 	@arg		GPIO_PUPD_PULLDOWN:	使用下拉电阻
// @param		otype:				引脚输出模式，只能选择以下参数：
// 	@arg		GPIO_OTYPE_PP:		推挽输出模式
// 	@arg		GPIO_OTYPE_OD:		开漏输出模式
// @param		speed:				引脚输出最大速度
// 	@arg		GPIO_OSPEED_2MHZ:	最大速度为10MHZ
// 	@arg		GPIO_OSPEED_25MHZ:	最大速度为2MHZ
// 	@arg		GPIO_OSPEED_50MHZ:	最大速度为50MHZ
// 	@arg		GPIO_OSPEED_200MHZ:	最大速度为200MHZ
// @param		alt_func_num:		选择的引脚复用功能，只能选择以下参数：
// 	@arg		GPIO_AF_1: 			TIMER0, TIMER1
// 	@arg		GPIO_AF_2: 			TIMER2, TIMER3, TIMER4
// 	@arg		GPIO_AF_3: 			TIMER7, TIMER8, TIMER9, TIMER10
// @return		void
// Sample usage:				tim_encoder_pin_init(pin);
//-------------------------------------------------------------------------------------------------------------------
static void tim_encoder_pin_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t speed, uint8_t otype, uint32_t alt_func_num, uint32_t pin)
{
    gpio_mode_set(gpio_periph, mode, pull_up_down, pin);
    gpio_output_options_set(gpio_periph, otype, speed, pin);    
    gpio_af_set(GPIOB, alt_func_num, pin);                     //提取对应IO索引
}
		
//-------------------------------------------------------------------------------------------------------------------
// @brief		 encoder 初始化
// @param		timer_periph: 				定时器外设号：TIMERx(x=0..4,7)         
// @param		gpio_periph					选择的端口：GPIOx(x = A,B,C,D,E,F,G,H,I)
// @param    	rcu_periph_enum periph:  	定时器时钟号：RCU_TIMERx (x=0,1,2,3,4,5,6,7,8,9,10,11,12,13): TIMER clock
// @param    speed: GPIO pin output max speed
             //GPIO_OSPEED_2MHZ: output max speed 2MHz 
             //GPIO_OSPEED_25MHZ: output max speed 25MHz 
             //GPIO_OSPEED_50MHZ: output max speed 50MHz
             //GPIO_OSPEED_200MHZ: output max speed 200MHz
// @param    pin: GPIO pin
             //one or more parameters can be selected which are shown as below:
             //GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @param    mode: gpio pin mode
             //GPIO_MODE_INPUT: input mode
             //GPIO_MODE_OUTPUT: output mode
             //GPIO_MODE_AF: alternate function mode
             //GPIO_MODE_ANALOG: analog mode
// @param    otype: gpio pin output mode
             //GPIO_OTYPE_PP: push pull mode
             //GPIO_OTYPE_OD: open drain mode
// @param    alt_func_num: GPIO pin af function
             //GPIO_AF_1: TIMER0, TIMER1
             //GPIO_AF_2: TIMER2, TIMER3, TIMER4
             //GPIO_AF_3: TIMER7, TIMER8, TIMER9, TIMER10
// @param		 RCU_TIMERx (x=0,1,2,3,4,5,6): TIMER clock
// @return		void
// Sample usage:				tim_encoder_init(uint32_t timer_periph, PULSEA_pin, TIM_ENC_PIN_enum);
//-------------------------------------------------------------------------------------------------------------------
void tim_encoder_init(uint32_t timer_periph, uint32_t PULSE_A_gpio_periph, uint32_t PULSE_A_speed, uint32_t PULSE_A_pin,
                      uint32_t PULSE_B_gpio_periph, uint32_t PULSE_B_speed, uint32_t PULSE_B_pin, uint16_t channel,
                      rcu_periph_enum periph, uint32_t mode, uint32_t pull_up_down, uint8_t otype, uint32_t alt_func_num)
{
    tim_encoder_pin_init(PULSE_A_gpio_periph, PULSE_A_speed, PULSE_A_pin, mode, pull_up_down, otype, alt_func_num);
    tim_encoder_pin_init(PULSE_B_gpio_periph, PULSE_B_speed, PULSE_B_pin, mode, pull_up_down, otype, alt_func_num);
    rcu_periph_clock_enable(periph);                                                // 使能时钟        
    timer_autoreload_value_config(timer_periph, 65535);                             // 装载自动重装载值
    timer_prescaler_config(timer_periph, 0, TIMER_PSC_RELOAD_NOW);                  // 装载预分频
				
    timer_input_trigger_source_select(timer_periph,TIMER_SMCFG_TRGSEL_CI0FE0);      //输入捕获IS0映射到CI0
    timer_input_trigger_source_select(timer_periph,TIMER_SMCFG_TRGSEL_CI1FE1);      //输入捕获IS1映射到CI1

    timer_channel_output_state_config(timer_periph, TIMER_CH_0 ,TIMER_CCX_ENABLE);     // 使能通道0
    timer_channel_output_state_config(timer_periph, TIMER_CH_1 ,TIMER_CCX_ENABLE);     // 使能通道1
    timer_slave_mode_select(timer_periph,TIMER_ENCODER_MODE2);                      // 从模式 编码器模式 2 选择
    timer_update_event_disable(timer_periph);                                       // 禁止UEV事件的产生
    timer_enable(timer_periph);                                                     // 使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		encoder 复位计数器
// @param		timer_periph: TIMERx(x=0..4,7)    
// Sample usage:				tim_encoder_rst(timer_periph);
//-------------------------------------------------------------------------------------------------------------------
void tim_encoder_rst(uint32_t timer_periph)
{
    timer_event_software_generate(timer_periph, TIMER_EVENT_SRC_UPG);               // 产生更新时间重装载计数器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取 encoder 计数器 并复位
// @param		timer_periph: TIMERx(x=0..4,7)    
// Sample usage:				temp = tim_encoder__get_count(timer_periph);
//-------------------------------------------------------------------------------------------------------------------
int16_t tim_encoder_get_count(uint32_t timer_periph)
{
	  int16_t temp = timer_counter_read(timer_periph);                               // 读取 encoder 计数器
	  timer_event_software_generate(timer_periph, TIMER_EVENT_SRC_UPG);              // 产生更新时间重装载计数器
	  return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置timer结构体
// @param		timer_initpara: 需要配置的结构体
// @param		freq_div		预分频值
// @param		period_temp		自动重装载值    
// Sample usage:				tim_struct_config(timer_initpara, 199, 65535);
//-------------------------------------------------------------------------------------------------------------------
void timer_struct_config(timer_parameter_struct *timer_initpara, uint16_t freq_div, uint16_t period_temp)
{
    timer_initpara->prescaler         = freq_div;
    timer_initpara->alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara->counterdirection  = TIMER_COUNTER_UP;
    timer_initpara->period            = period_temp;
    timer_initpara->clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara->repetitioncounter = 0;
}
