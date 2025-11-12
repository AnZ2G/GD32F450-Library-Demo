/*********************************************************************************************************************
* @file				KEY.c
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-26
********************************************************************************************************************/

#include "KEY.h"
#include "headfile.h"
#include "systick.h"

/* 类型定义 */

/* 按键端口定义 */
static uint32_t GPIO_PORT_KEY[KEYn] = {KEY_UP_GPIO_PORT, KEY_DOWN_GPIO_PORT, KEY_LEFT_GPIO_PORT, KEY_RIGHT_GPIO_PORT, KEY_SET_GPIO_PORT, KEY_BACK_GPIO_PORT};

/* 按键引脚定义 */
static uint32_t GPIO_PIN_KEY[KEYn] = {KEY_UP_PIN, KEY_DOWN_PIN, KEY_LEFT_PIN, KEY_RIGHT_PIN, KEY_SET_PIN, KEY_BACK_PIN};

/* 按键时钟定义 */
static rcu_periph_enum GPIO_CLK_KEY[KEYn] = {KEY_UP_GPIO_CLK, KEY_DOWN_GPIO_CLK, KEY_LEFT_GPIO_CLK, KEY_RIGHT_GPIO_CLK, KEY_SET_GPIO_CLK, KEY_BACK_GPIO_CLK};

/* 拨码开关端口定义 */
static uint32_t GPIO_PORT_SWITCH[KEYn] = {SWITCH1_GPIO_PORT, SWITCH2_GPIO_PORT, SWITCH3_GPIO_PORT, SWITCH4_GPIO_PORT};

/* 拨码开关引脚定义 */
static uint32_t GPIO_PIN_SWITCH[KEYn] = {SWITCH1_PIN, SWITCH2_PIN, SWITCH3_PIN, SWITCH4_PIN};

/* 拨码开关时钟定义 */
static rcu_periph_enum GPIO_CLK_SWITCH[KEYn] = {SWITCH1_GPIO_CLK, SWITCH2_GPIO_CLK, SWITCH3_GPIO_CLK, SWITCH4_GPIO_CLK};

//-------------------------------------------------------------------------------------------------------------------
// @brief		按键初始化
// @param		keynum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		void
// Sample usage:			KEY_Init(KEY_UP);//初始化LED1
//-------------------------------------------------------------------------------------------------------------------
void KEY_Init(key_typedef_enum keynum)
{
    if(keynum != KEY_ALL)
    {
        /* 使能LED时钟 */
        rcu_periph_clock_enable(GPIO_CLK_KEY[keynum]);

        /* 配置LED GPIO引脚 */ 
        gpio_input_init(GPIO_PORT_KEY[keynum], GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_KEY[keynum]);
    }
    else
    {
        rcu_periph_clock_enable(RCU_GPIOD);
        gpio_input_init(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取单个按键输入状态
// @param		keynum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		uint8
// Sample usage:			if(KEY_PRESS == KEY_Get_Input(KEY_UP)) LED_On(LED1);//如果UP键按下，点亮LED1
//-------------------------------------------------------------------------------------------------------------------
uint8_t KEY_Get_Input(key_typedef_enum keynum)
{
    return gpio_get_input(GPIO_PORT_KEY[keynum], GPIO_PIN_KEY[keynum]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取输入的按键值
// @param		mode				读取模式
// @return		uint8
// Sample usage:			if(1 == KEY_Read(1)) LED_On(LED1);//如果读取的按键值为1（up键），点亮LED1
//-------------------------------------------------------------------------------------------------------------------
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
        key_up=1;      //支持连按
    }
    if(key_up && (KEY_PRESS == KEY_Get_Input(KEY_UP) || KEY_PRESS == KEY_Get_Input(KEY_DOWN) || KEY_PRESS == KEY_Get_Input(KEY_LEFT)||
                  KEY_PRESS == KEY_Get_Input(KEY_RIGHT) || KEY_PRESS == KEY_Get_Input(KEY_SET) || KEY_PRESS == KEY_Get_Input(KEY_BACK)))
    {
        delay_1ms(20);  //消抖
        key_up=0;
        if(KEY_PRESS == KEY_Get_Input(KEY_UP)) return 1;			//up
        else if(KEY_PRESS == KEY_Get_Input(KEY_LEFT)) return 2;		//left  
        else if(KEY_PRESS == KEY_Get_Input(KEY_RIGHT)) return 3;	//right
        else if(KEY_PRESS == KEY_Get_Input(KEY_DOWN)) return 4;		//down
        else if(KEY_PRESS == KEY_Get_Input(KEY_SET)) return 5;		//set  
        else if(KEY_PRESS == KEY_Get_Input(KEY_BACK)) return 6;		//back
    }
    if((KEY_RELEASE == KEY_Get_Input(KEY_UP) || KEY_RELEASE == KEY_Get_Input(KEY_DOWN) || KEY_RELEASE == KEY_Get_Input(KEY_LEFT)||
        KEY_RELEASE == KEY_Get_Input(KEY_RIGHT) || KEY_RELEASE == KEY_Get_Input(KEY_SET) || KEY_RELEASE == KEY_Get_Input(KEY_BACK)))    
    {
        key_up=1;
    }
    return 0;   //无按键按下
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		按键初始化
// @param		switchnum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		void
// Sample usage:			SWITCH_Init(SWITCH1);//初始化拨码开关1
//-------------------------------------------------------------------------------------------------------------------
void SWITCH_Init(switch_typedef_enum switchnum)
{
    if(switchnum != SWITCH_ALL)
    {
        /* 使能LED时钟 */
        rcu_periph_clock_enable(GPIO_CLK_SWITCH[switchnum]);

        /* 配置LED GPIO引脚 */ 
        gpio_input_init(GPIO_PORT_SWITCH[switchnum], GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_SWITCH[switchnum]);
    }
    else
    {
        rcu_periph_clock_enable(RCU_GPIOG);
        gpio_input_init(GPIOG, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取单个按键输入状态
// @param		keynum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		uint8
// Sample usage:			if(SWITCH_ON == SWITCH_Get_Input(SWITCH)) LED_On(LED1);//如果拨码开关1打开，点亮LED
//-------------------------------------------------------------------------------------------------------------------
uint8_t SWITCH_Get_Input(switch_typedef_enum switchnum)
{
    return gpio_get_input(GPIO_PORT_SWITCH[switchnum], GPIO_PIN_SWITCH[switchnum]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		使能按键中断
// @param		keynum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		void
// Sample usage:			KEY_EXTI_Enable(KEY_UP);//使能up键中断（下降沿触发）
//-------------------------------------------------------------------------------------------------------------------
void KEY_EXTI_Enable(key_typedef_enum keynum)
{
    if(keynum != KEY_ALL)
    {
        exti_enable(GPIO_PORT_KEY[keynum], GPIO_PIN_KEY[keynum], FALLING);
    }
    else
    {
        exti_enable(GPIOD, GPIO_PIN_8, FALLING);
        exti_enable(GPIOD, GPIO_PIN_9, FALLING);
        exti_enable(GPIOD, GPIO_PIN_10, FALLING);
        exti_enable(GPIOD, GPIO_PIN_11, FALLING);
        exti_enable(GPIOD, GPIO_PIN_12, FALLING);
        exti_enable(GPIOD, GPIO_PIN_13, FALLING);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		失能按键中断
// @param		keynum				选择的按键：KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEYSET,KEY_BACK
// @return		void
// Sample usage:			KEY_EXTI_Disanle(KEY_UP);//失能up键中断
//-------------------------------------------------------------------------------------------------------------------
void KEY_EXTI_Disable(key_typedef_enum keynum)
{
    if(keynum != KEY_ALL)
    {
        exti_disable(GPIO_PIN_KEY[keynum]);
    }
    else
    {
        exti_disable(GPIO_PIN_8);
        exti_disable(GPIO_PIN_9);
        exti_disable(GPIO_PIN_10);
        exti_disable(GPIO_PIN_11);
        exti_disable(GPIO_PIN_12);
        exti_disable(GPIO_PIN_13);
    }
}
