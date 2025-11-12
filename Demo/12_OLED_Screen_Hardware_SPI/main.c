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
#include "gd32f450_board.h"
#include "systick.h"
#include <stdio.h>
#include "oled.h"
#include "oledfont.h"

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 配置LED */
    led_init(LED1);
    led_init(LED2);

    /* 初始化OLED */
    OLED_Init();
    delay_1ms(200);

    /* 屏幕闪烁 */
    OLED_Clear(1);
    delay_1ms(500);
    OLED_Clear(0);
    delay_1ms(500);
    led_on(LED1);

    /* OLED写入“合肥炜煌” */
    OLED_show_chinese(chinese_buffer[0],18);
    OLED_show_chinese(chinese_buffer[1],19);
    OLED_show_chinese(chinese_buffer[2],20);
    OLED_show_chinese(chinese_buffer[3],21);

    /* OLED写入"Hello" */
    OLED_show_ascii(acsii_buffer[40],33);
    OLED_show_ascii_half(acsii_buffer[69],33);
    OLED_show_ascii(acsii_buffer[76],34);
    OLED_show_ascii_half(acsii_buffer[76],34);
    OLED_show_ascii(acsii_buffer[79],35);

    /* OLED写入"World" */
    OLED_show_ascii(acsii_buffer[55],36);
    OLED_show_ascii_half(acsii_buffer[79],36);
    OLED_show_ascii(acsii_buffer[82],37);
    OLED_show_ascii_half(acsii_buffer[76],37);
    OLED_show_ascii(acsii_buffer[68],38);

    /* OLED显示 */
    OLED_Display();

    while(1)
    {
        /* OLED显示"!" */
        OLED_show_ascii_half(acsii_buffer[1],38);
        OLED_Display();
        delay_1ms(1000);

        /* OLED清除"!" */
        OLED_show_ascii_half(acsii_buffer[0],38);
        OLED_Display();
        delay_1ms(1000);
    }
}

