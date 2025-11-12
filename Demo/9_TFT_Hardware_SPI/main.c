/*********************************************************************************************************************
* @file				main.c
* @brief			OLED（硬件SPI）
* @author			HeFei WeiHuang
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-4-10
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何应用外部OLED库和SPI库
配置OLED，进行一定的显示操作

-在GD32F450学习板上，PB12连接CS引脚，PB14连接DC引脚，
PB15连接SD引脚，PB13连接CK引脚。使用外部OLED库和SPI库
对SPI进行配置，并通过SPI通讯函数控制OLED屏显示情况。

-将OLED屏接入学习板对应位置后，接通电源，OLED屏闪烁
一次，随后显示“合肥炜煌 Hello World！”字样。“！”闪烁
，间隔1s。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "headfile.h"
#include "LED.h"
#include "TFT.h"
int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    LED_Init(LED_ALL);
    delay_1ms(500);
    LED_Off(LED_ALL);
    delay_1ms(500);

    TFT_SPI_Init();
    LED_On(LED1);

 	delay_1ms(1000);

    TFT_Init(2);
    LED_On(LED2);

    TFTSPI_CLS(0x0000);
    LED_On(LED3);
    while(1)
    {
//    TFT_SPI_Init();
//    LED_On(LED1);

    TFT_Init(2);
    LED_On(LED2);

//    TFTSPI_Fill_Area(0, 0, TFT18H, TFT18W, YELLOW);
    TFTSPI_CLS(0xffff);
    LED_On(LED3);

    delay_1ms(1000);
    LED_Off(LED_ALL);
    }
}

