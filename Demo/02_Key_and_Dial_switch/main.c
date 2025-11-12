/*********************************************************************************************************************
* @file				main.c
* @brief			按键和拨码开关
* @author			HeFei WeiHuang
* @version			2.1
* @Target core		GD32F450ZIT6
* @date				2021-4-8
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何配置按键、拨码开
关和LED的GPIO，并通过检测按键GPIO输入信号控制LED亮灭。

-在GD32F450学习板上，PD2连接LED1，PD0连接LED2，PD1
连接LED3。

-PD8、PD9、PD10、PD11、PD12、PD13连接按键。
在按下按键D8、D11时LED1会改变亮灭状态；
在按下按键D9、D10时LED2会改变亮灭状态；
在按下按键D12、D13时LED3会改变亮灭状态；
按住按键时，LED会闪烁。

-PG2、PG3、PG4、PG5连接拨码开关，在拨动拨码开关后，
LED闪烁的周期会改变。打开的拨码开关越多，LED闪烁周期
越长。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include "headfile.h"
#include "LED.h"
#include "KEY.h"
#include <stdio.h>

/* 定义LED号 */
uint8_t led_num;

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 初始化LED */
    LED_Init(LED_ALL);

    /* LED全亮 */
    LED_On(LED_ALL);
		
    /* 初始化按键 */
    KEY_Init(KEY_ALL);

    /* 开启按键中断 */
    KEY_EXTI_Enable(KEY_ALL);

    /* 初始化拨码开关 */
    SWITCH_Init(SWITCH_ALL);

    /* 定义闪烁时间 */
    uint8_t time;
    time = 0;
    led_num = 1;

    while(1)
    {
        /* 检测拨码开关1是否按下 */
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH1))
        {
            /* 闪烁周期延长(2倍) */
            time |= 1;
        }
        /* 否则闪烁周期还原 */
        else time = 0;

        /* 检测拨码开关2是否按下 */
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH2))
        {
            /* 闪烁周期延长（4倍） */
            time |= (1<<1);
        }
        /* 否则闪烁周期还原 */
        else time &= ~(1<<1);

        /* 检测拨码开关3是否按下 */
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH3))
        {
            /* 闪烁周期延长（8倍） */
            time |= (1<<2);
        }
        /* 否则闪烁周期还原 */
        else time &= ~(1<<2);

        /* 检测拨码开关4是否按下 */
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH4))
        {
            /* 闪烁周期延长（16倍） */
            time |= (1<<3);
        }
        /* 否则闪烁周期还原 */
        else time &= ~(1<<3);

        if(time!=0)
        {
            if(led_num == 1)
            {
                LED_Toggle(LED1);
                delay_1ms(time*100);
            }
            else if(led_num == 2)
            {
                LED_Toggle(LED2);
                delay_1ms(time*100);
            }
            else if(led_num == 3)
            {
                LED_Toggle(LED3);
                delay_1ms(time*100);
            }
        }
    }
}


/* 按键中断函数 */
void EXTI5_9_IRQHandler(void)
{
    /* back键中断函数 */
    if(RESET != exti_interrupt_flag_get(EXTI_8))
    {

        led_num = 3;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_8))
        exti_interrupt_flag_clear(EXTI_8);
    }

    /* up键中断函数 */
    else if(RESET != exti_interrupt_flag_get(EXTI_9))
    {

        led_num = 2;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_9))
        exti_interrupt_flag_clear(EXTI_9);
    }
}

/* 按键中断函数 */
void EXTI10_15_IRQHandler(void)
{
    /* down键中断函数 */
    if(RESET != exti_interrupt_flag_get(EXTI_10))
    {

        led_num = 2;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_10))
        exti_interrupt_flag_clear(EXTI_10);
    }

    /* right键中断函数 */
    else if(RESET != exti_interrupt_flag_get(EXTI_11))
    {

        led_num = 3;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_11))
        exti_interrupt_flag_clear(EXTI_11);
    }

    /* left键中断函数 */
    if(RESET != exti_interrupt_flag_get(EXTI_12))
    {

        led_num = 1;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_12))
        exti_interrupt_flag_clear(EXTI_12);
    }

    /* set键中断函数 */
    else if(RESET != exti_interrupt_flag_get(EXTI_13))
    {

        led_num = 1;

        while(0==gpio_get_input(GPIOD,GPIO_PIN_13))
        exti_interrupt_flag_clear(EXTI_13);
    }
}
