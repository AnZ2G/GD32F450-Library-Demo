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

-此例程基于GD32F450学习板, 展示如何配置按键关和
LED的GPIO，并通过读取按键输入信号控制LED亮灭。

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

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 初始化LED */
    LED_Init(LED_ALL);
    delay_1ms(500);

    /* LED全灭 */
    LED_Off(LED_ALL);

    /* 初始化按键 */
    KEY_Init(KEY_ALL);

    while(1)
    {
        /* 当up键被按下 */
        if(1 == KEY_Read(1))
        {
            LED_On(LED1);
            LED_Off(LED2);
            LED_Off(LED3);
        }
        /* 当left键被按下 */
        else if(2 == KEY_Read(1))
        {
            LED_Off(LED1);
            LED_On(LED2);
            LED_Off(LED3);
        }
        /* 当right键被按下 */
        else if(3 == KEY_Read(1))
        {
            LED_On(LED1);
            LED_On(LED2);
            LED_Off(LED3);
        }
        /* 当down键被按下 */
        else if(4 == KEY_Read(1))
        {
            LED_Off(LED1);
            LED_Off(LED2);
            LED_On(LED3);
        }
        /* 当set键被按下 */
        else if(5 == KEY_Read(1))
        {
            LED_On(LED1);
            LED_Off(LED2);
            LED_On(LED3);
        }
        /* 当back键被按下 */
        else if(6 == KEY_Read(1))
        {
            LED_Off(LED1);
            LED_On(LED2);
            LED_On(LED3);
        }
        /* 没有键被按下 */
        else if(0 == KEY_Read(1))
        {
            LED_Off(LED_ALL);
        }
    }
}
