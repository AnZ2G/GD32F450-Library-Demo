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
#include "headfile.h"
#include "HCI.h"
#include "key.h"
int main(void)
{
    /* 配置滴答计时器 */
    systick_config();
	int x=1400;
    /* 配置LED */
    led_init(LED1);
    led_init(LED2);
    led_init(LED3);

//    uint8_t write_buffer[144] = {0,};
//    flash_WriteSector_uint8_t(2, 0, write_buffer, 144);

    /* 配置I2C */
    i2c_config(400000, 0x00);
  	HCI_init();
	  HCI_handle();

    /* 配置GPIO时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* 配置PA3引脚及复用功能，作为TIMER4通道3 */
    gpio_output_init(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_3);


		while(1)
		{
			OLED_ShowNum(  0, 0, 10000+x, 5,16 );
			if(KEY_Read(1)==1)
			{
				x=x+10;
			}
			if(KEY_Read(1)==2)
			{
				x=x-10;
			}
			pwm_init(RCU_TIMER4, TIMER4, TIMER_CH_3, 99, 19999, x);
		}
}

