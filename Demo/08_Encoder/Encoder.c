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
#include "headfile.h"

uint16_t temp1,temp2,temp3,temp4 = 0;
int16_t value1,value2 = 0;

/* 类型定义 */
static uint32_t GPIO_PORT_Encoder[Encodern] = {Encoder1_GPIO_PORT, Encoder2_GPIO_PORT};

static uint32_t GPIO_PIN_A_Encoder[Encodern] = {Encoder1_PIN_A, Encoder2_PIN_A};

static uint32_t GPIO_PIN_B_Encoder[Encodern] = {Encoder1_PIN_B, Encoder2_PIN_B};

static rcu_periph_enum GPIO_CLK_Encoder[Encodern] = {Encoder1_GPIO_CLK, Encoder2_GPIO_CLK};

static uint32_t GPIO_PIN_AF_Encoder[Encodern] = {Encoder1_GPIO_AF, Encoder2_GPIO_AF};

static uint32_t TIMER_PERIPH_Encoder[Encodern] = {Encoder1_TIMER, Encoder2_TIMER};

static rcu_periph_enum TIMER_CLK_Encoder[Encodern] = {Encoder1_TIMER_CLK, Encoder2_TIMER_CLK};

static uint32_t CHANNAL_A_Encoder[Encodern] = {Encoder1_CHANNAL_A, Encoder2_CHANNAL_A};

static uint32_t INT_CHANNAL_Encoder[Encodern] = {Encoder1_INT_CHANNAL, Encoder2_INT_CHANNAL};

static uint32_t INT_FLAG_Encoder[Encodern] = {Encoder1_INT_FLAG, Encoder2_INT_FLAG};

static uint32_t TIMER_NVIC_Encoder[Encodern] = {Encoder1_TIMER_NVIC, Encoder2_TIMER_NVIC};

//-------------------------------------------------------------------------------------------------------------------
// @brief		编码器初始化
// @param		Encodernum			选择的编码器：Encoder1、2，Encoder_ALL
// @param		mode				选择的编码器编码模式：Mode1(带方向)、2(正交)
// @return		void
// Sample usage:				Encoder_Init(Encoder_ALL, Mode1);//初始化编码器1和编码器2，编码模式为带方向
//-------------------------------------------------------------------------------------------------------------------
void Encoder_Init(encoder_typedef_enum Encodernum, encodermode_typedef_enum mode)
{
    encoder_typedef_enum encodernum;
    if(Encodernum == Encoder_ALL) encodernum = Encoder1;
    else encodernum = Encodernum;

    if(encodernum == Encoder1 && mode == Mode1)
    {
        rcu_periph_clock_enable(RCU_SYSCFG);
        rcu_periph_clock_enable(GPIO_CLK_Encoder[encodernum]);

        /* 配置A引脚及复用功能，作为TIMER通道 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_A_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_A_Encoder[encodernum]);
	    /* 配置B引脚，读取方向 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_B_Encoder[encodernum]);

        /* 配置定时器时钟 */
        rcu_periph_clock_enable(TIMER_CLK_Encoder[encodernum]);
    
        /* 配置定时器时钟预分频器 */
        rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

        /* 初始化TIMER */
        timer_deinit(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器结构体并配置定时器TIMER */
        timer_parameter_struct timer_initpara;
        timer_initpara.prescaler         = 199;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = CollectionTime - 1;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_initpara.repetitioncounter = 0;
        timer_init(TIMER_PERIPH_Encoder[encodernum],&timer_initpara);

  	    timer_ic_parameter_struct timer_icinitpara;
       	timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
  	    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
  	    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
        timer_icinitpara.icfilter = 0;
        timer_input_capture_config(TIMER_PERIPH_Encoder[encodernum], CHANNAL_A_Encoder[encodernum], &timer_icinitpara);

        /* 使能定时器重装载 */
        timer_auto_reload_shadow_enable(TIMER_PERIPH_Encoder[encodernum]);

        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[encodernum], INT_CHANNAL_Encoder[encodernum]);
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[encodernum], TIMER_INT_FLAG_UP);
        timer_interrupt_enable(TIMER_PERIPH_Encoder[encodernum], TIMER_INT_UP);
        timer_interrupt_enable(TIMER_PERIPH_Encoder[encodernum], INT_CHANNAL_Encoder[encodernum]);

        nvic_irq_enable(TIMER_NVIC_Encoder[encodernum], 0, 0);
        /* 使能定时器TIMER2 */
        timer_enable(TIMER_PERIPH_Encoder[encodernum]);
    }
    else if(encodernum == Encoder1 && mode == Mode2)
    {
        rcu_periph_clock_enable(GPIO_CLK_Encoder[encodernum]);

        /* 配置A,B引脚及复用功能，作为TIMER通道 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_A_Encoder[encodernum]);
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_B_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_A_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_B_Encoder[encodernum]);

        /* 定义定时器结构体 */
        timer_parameter_struct timer_initpara;

        /* 配置定时器时钟 */
        rcu_periph_clock_enable(TIMER_CLK_Encoder[encodernum]);

        /* 配置定时器时钟预分频器 */
        rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

        /* 初始化TIMER2 */
        timer_deinit(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器结构体并配置定时器 */
        timer_initpara.prescaler         = 9;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 5000;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_initpara.repetitioncounter = 0;
        timer_init(TIMER_PERIPH_Encoder[encodernum],&timer_initpara);

        /* 使能定时器重装载 */
        timer_auto_reload_shadow_enable(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器正交解码器 */
        timer_quadrature_decoder_mode_config(TIMER_PERIPH_Encoder[encodernum],TIMER_ENCODER_MODE2,TIMER_IC_POLARITY_RISING,TIMER_IC_POLARITY_RISING);

        /* 使能定时器 */
        timer_enable(TIMER_PERIPH_Encoder[encodernum]);

        timer_interrupt_init(TIMER1, CollectionTime - 1);
    }

    if(Encodernum == Encoder_ALL) encodernum = Encoder2;

    if(encodernum == Encoder2 && mode == Mode1)
    {
        rcu_periph_clock_enable(RCU_SYSCFG);
        rcu_periph_clock_enable(GPIO_CLK_Encoder[encodernum]);

        /* 配置A引脚及复用功能，作为TIMER通道 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_A_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_A_Encoder[encodernum]);
	    /* 配置B引脚，读取方向 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_B_Encoder[encodernum]);

        /* 配置定时器时钟 */
        rcu_periph_clock_enable(TIMER_CLK_Encoder[encodernum]);
    
        /* 配置定时器时钟预分频器 */
        rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

        /* 初始化TIMER2 */
        timer_deinit(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器结构体并配置定时器 */
        timer_parameter_struct timer_initpara;
        timer_initpara.prescaler         = 199;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = CollectionTime - 1;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_initpara.repetitioncounter = 0;
        timer_init(TIMER_PERIPH_Encoder[encodernum],&timer_initpara);

  	    timer_ic_parameter_struct timer_icinitpara;
       	timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
  	    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
  	    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
        timer_icinitpara.icfilter = 0;
        timer_input_capture_config(TIMER_PERIPH_Encoder[encodernum], CHANNAL_A_Encoder[encodernum], &timer_icinitpara);

        /* 使能定时器重装载 */
        timer_auto_reload_shadow_enable(TIMER_PERIPH_Encoder[encodernum]);

        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[encodernum], INT_CHANNAL_Encoder[encodernum]);
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[encodernum], TIMER_INT_FLAG_UP);
        timer_interrupt_enable(TIMER_PERIPH_Encoder[encodernum], TIMER_INT_UP);
        timer_interrupt_enable(TIMER_PERIPH_Encoder[encodernum], INT_CHANNAL_Encoder[encodernum]);

        nvic_irq_enable(TIMER_NVIC_Encoder[encodernum], 0, 0);
        /* 使能定时器 */
        timer_enable(TIMER_PERIPH_Encoder[encodernum]);
    }
    else if(encodernum == Encoder2 && mode == Mode2)
    {
        rcu_periph_clock_enable(GPIO_CLK_Encoder[encodernum]);

        /* 配置A,B引脚及复用功能，作为TIMER通道 */
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_A_Encoder[encodernum]);
        gpio_mode_set(GPIO_PORT_Encoder[encodernum], GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_B_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_A_Encoder[encodernum]);
        gpio_af_set(GPIO_PORT_Encoder[encodernum], GPIO_PIN_AF_Encoder[encodernum], GPIO_PIN_B_Encoder[encodernum]);

        /* 定义定时器结构体 */
        timer_parameter_struct timer_initpara;

        /* 配置定时器时钟 */
        rcu_periph_clock_enable(TIMER_CLK_Encoder[encodernum]);

        /* 配置定时器时钟预分频器 */
        rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

        /* 初始化TIMER2 */
        timer_deinit(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器结构体并配置定时器 */
        timer_initpara.prescaler         = 9;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 5000;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_initpara.repetitioncounter = 0;
        timer_init(TIMER_PERIPH_Encoder[encodernum],&timer_initpara);

        /* 使能定时器重装载 */
        timer_auto_reload_shadow_enable(TIMER_PERIPH_Encoder[encodernum]);

        /* 配置定时器正交解码器 */
        timer_quadrature_decoder_mode_config(TIMER_PERIPH_Encoder[encodernum],TIMER_ENCODER_MODE2,TIMER_IC_POLARITY_RISING,TIMER_IC_POLARITY_RISING);

        /* 使能定时器 */
        timer_enable(TIMER_PERIPH_Encoder[encodernum]);

        timer_interrupt_init(TIMER1, CollectionTime - 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取编码器值
// @return		int16
// Sample usage:				int16 value;value = Encoder_Read(Encoder1);
//-------------------------------------------------------------------------------------------------------------------
int16_t Encoder_Read(encoder_typedef_enum Encodernum)
{
    uint16_t value;
    if(Encodernum == Encoder1)value = value1;
    else if(Encodernum == Encoder2) value = value2;
    return value;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		定时器2中断函数（内部函数）
// @return		void
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void TIMER2_IRQHandler(void)
{
    if (RESET != timer_interrupt_flag_get(TIMER_PERIPH_Encoder[Encoder1], INT_FLAG_Encoder[Encoder1])) 
    {
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[Encoder1], INT_FLAG_Encoder[Encoder1]);
        temp1++;
    }
    else if (RESET != timer_interrupt_flag_get(TIMER_PERIPH_Encoder[Encoder1], TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[Encoder1], TIMER_INT_FLAG_UP);
        value1 = temp1;
        temp1 = 0;
        if(gpio_input_bit_get(GPIO_PORT_Encoder[Encoder1], GPIO_PIN_B_Encoder[Encoder1])) value1 = -value1;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		定时器4中断函数（内部函数）
// @return		void
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void TIMER4_IRQHandler(void)
{
    if (RESET != timer_interrupt_flag_get(TIMER_PERIPH_Encoder[Encoder2], INT_FLAG_Encoder[Encoder2])) 
    {
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[Encoder2], INT_FLAG_Encoder[Encoder2]);
        temp2++;
    }
    else if (RESET != timer_interrupt_flag_get(TIMER_PERIPH_Encoder[Encoder2], TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER_PERIPH_Encoder[Encoder2], TIMER_INT_FLAG_UP);
        value2 = temp2;
        temp2 = 0;
        if(gpio_input_bit_get(GPIO_PORT_Encoder[Encoder2], GPIO_PIN_B_Encoder[Encoder2])) value2 = -value2;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		定时器1中断函数（内部函数）
// @return		void
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void TIMER1_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER1,TIMER_INT_FLAG_UP) != RESET)
    {
        timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
        value1 = timer_counter_read(TIMER2)-10000;
        timer_counter_value_config(TIMER2,10000);
        value2 = 10000-timer_counter_read(TIMER4);
        timer_counter_value_config(TIMER4,10000);
    }
}
