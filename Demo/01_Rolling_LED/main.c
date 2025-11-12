/*********************************************************************************************************************
* @file				main.c
* @brief			流水灯（板载资源）
* @author			HeFei WeiHuang
* @version			2.0
* @Target core		GD32F450ZIT6
* @date				2021-4-2
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何应用板载资源库
配置LED，并通过指令控制LED亮灭。

-在GD32F450学习板上，PD2连接LED1，PD0连接LED2，PD1
连接LED3。

-LED1、LED2、LED3顺序点亮，间隔1s。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include "LED.h"
#include <stdio.h>

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 配置LED */
    LED_Init(LED_ALL);
    delay_1ms(500);

    LED_Toggle(LED_ALL);
    delay_1ms(500);

    LED_Toggle(LED_ALL);
    delay_1ms(500);

    LED_Toggle(LED_ALL);
    delay_1ms(500);

    while(1)
    {
        /* LED1亮，LED2、LED3、LED4灭 */
        LED_On(LED1);
        LED_Off(LED2);
        LED_Off(LED3);
        LED_Off(LED4);
        delay_1ms(1000);

        /* LED2亮，LED1、LED3、LED4灭 */
        LED_On(LED2);
        LED_Off(LED1);
        LED_Off(LED3);
        LED_Off(LED3);
        delay_1ms(1000);

        /* LED3亮，LED1、LED2、LED4灭 */
        LED_On(LED3);
        LED_Off(LED1);
        LED_Off(LED2);
        LED_Off(LED4);
        delay_1ms(1000);

        /* LED4亮，LED1、LED2、LED3灭 */
        LED_On(LED4);
        LED_Off(LED1);
        LED_Off(LED2);
        LED_Off(LED3);
        delay_1ms(1000);
    }
}
