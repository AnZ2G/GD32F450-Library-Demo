/*********************************************************************************************************************
* @file				LED.c
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_V1.0
* @date				2021-8-28
********************************************************************************************************************/

#include "LED.h"
#include "headfile.h"

/* 类型定义 */
static uint32_t GPIO_PORT_LED[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,LED3_GPIO_PORT, LED4_GPIO_PORT};

static uint32_t GPIO_PIN_LED[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

static rcu_periph_enum GPIO_CLK_LED[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED初始化
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_init(LED1);//初始化LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_Init(led_typedef_enum lednum)
{
    if(lednum != LED_ALL)
    {
        /* 使能LED时钟 */
        rcu_periph_clock_enable(GPIO_CLK_LED[lednum]);

        /* 配置LED GPIO引脚 */ 
        gpio_output_init(GPIO_PORT_LED[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_LED[lednum]);

        /* 引脚设置低电平，LED点亮 */
        gpio_set_output(GPIO_PORT_LED[lednum], GPIO_PIN_LED[lednum], LED_ON);
    }
    else
    {
        rcu_periph_clock_enable(RCU_GPIOD);
        gpio_output_init(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
        gpio_set_output(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED熄灭
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_off(LED1);//熄灭LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_Off(led_typedef_enum lednum)
{
    if(lednum != LED_ALL)
    {
        gpio_set_output(GPIO_PORT_LED[lednum], GPIO_PIN_LED[lednum], LED_OFF);
    }
    else
    {
        gpio_set_output(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED点亮
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_on(LED1);//点亮LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_On(led_typedef_enum lednum)
{
    if(lednum != LED_ALL)
    {
        gpio_set_output(GPIO_PORT_LED[lednum], GPIO_PIN_LED[lednum], LED_ON);
    }
    else
    {
        gpio_set_output(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED状态切换
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_toggle(LED1);//切换LED1状态，原先点亮则熄灭，原先熄灭则点亮
//-------------------------------------------------------------------------------------------------------------------
void LED_Toggle(led_typedef_enum lednum)
{
    if(lednum != LED_ALL)
    {
        gpio_toggle(GPIO_PORT_LED[lednum], GPIO_PIN_LED[lednum]);
    }
    else
    {
        gpio_toggle(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    }
}
