/*********************************************************************************************************************
* @file				board.c
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-26
********************************************************************************************************************/

#include "board.h"

/* 类型定义 */
static uint32_t GPIO_PORT_LED[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,
                                   LED3_GPIO_PORT};

static uint32_t GPIO_PIN_LED[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};

static rcu_periph_enum GPIO_CLK_LED[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, 
                                         LED3_GPIO_CLK};

static uint32_t GPIO_PORT_KEY[KEYn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,
                                   LED3_GPIO_PORT};

static uint32_t GPIO_PIN_KEY[KEYn] = {LED1_PIN, LED2_PIN, LED3_PIN};

static rcu_periph_enum GPIO_CLK_KEY[KEYn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, 
                                         LED3_GPIO_CLK};

static uint32_t GPIO_PORT_SWITCH[SWITCHn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,
                                   LED3_GPIO_PORT};

static uint32_t GPIO_PIN_SWITCH[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};

static rcu_periph_enum GPIO_CLK_SWITCH[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, 
                                         LED3_GPIO_CLK};

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED初始化
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_init(LED1);//初始化LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_Init(led_typedef_enum lednum)
{
    /* 使能LED时钟 */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);

    /* 配置LED GPIO引脚 */ 
    gpio_mode_set(GPIO_PORT_LED[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT_LED[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN[lednum]);

    /* 引脚设置低电平，LED点亮 */
    GPIO_BC(GPIO_PORT_LED[lednum]) = GPIO_PIN[lednum];
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED熄灭
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_off(LED1);//熄灭LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_Off(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT_LED[lednum]) = GPIO_PIN[lednum];
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED点亮
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_on(LED1);//点亮LED1
//-------------------------------------------------------------------------------------------------------------------
void LED_On(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT_LED[lednum]) = GPIO_PIN[lednum];
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		LED状态切换
// @param		lednum				选择的LED：LED1,LED2,LED3
// @return		void
// Sample usage:			led_toggle(LED1);//切换LED1状态，原先点亮则熄灭，原先熄灭则点亮
//-------------------------------------------------------------------------------------------------------------------
void LED_Toggle(led_typedef_enum lednum)
{
    GPIO_TG(GPIO_PORT_LED[lednum]) = GPIO_PIN[lednum];
}
