/*********************************************************************************************************************
* @file				main.c
* @brief			DCI摄像头
* @author			HeFei WeiHuang
* @version			1.1
* @Target core		GD32F450ZIT6
* @date				2021-4-10
********************************************************************************************************************/

/*********************************************************************************************************************
* 例程说明：

-此例程基于GD32F450学习板, 展示如何配置总钻风摄像头，
并通过F450的DCI资源获取图像，用有线串口发送至电脑。

-在GD32F450学习板上，PA6连接摄像头PCLK引脚，PA10
连接摄像头TXD引脚，PA9连接摄像头RXD引脚。PA4连接
摄像头NREF引脚，PB7连接摄像头VSY引脚，PC6、PC7、
PC8、PC9、PC11、PB6、PB8、PB9分别连接摄像头D0至
D7引脚。PC10连接有线串口TXD引脚，PC11连接有线串口
RXD引脚。

-打开电源后，串口向电脑发送测试程序。当收到“1、“2”、
“3”信息时，说明摄像头配置完成。随后持续向电脑发送
摄像头采集图像。可以使用山外多功能调试助手的摄像头图
像分析功能查看摄像头采集图像。
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "gd32f450_board.h"
#include "headfile.h"
#include "oled.h"
#include "systick.h"
#include <stdio.h>
#include "SEEKFREE_MT9V03X.h"
#include "HCI.h"
#include "TFT.h"


extern uint8_t mt9v03x_finish_flag;
uint16_t flame_cnt;

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 配置LED并点亮LED1 */
    led_init(LED1);
    led_init(LED2);
    led_init(LED3);
    led_on(LED1);

    /* 配置PC10为TXD引脚 */
    uart_tx_init(GPIOB, GPIO_PIN_10, RCU_GPIOB);

    /* 配置PC11为RXD引脚 */
    uart_rx_init(GPIOB, GPIO_PIN_11, RCU_GPIOB);

    /* 配置UART，使用USART2，波特率设置为115200 */
    uart_init(RCU_USART2, USART2, 115200U);    
//    i2c_config(400000, 0x00);
//    OLED_Init();
//    Clr_Screen(8);
    /* 向串口发送验证信息 */
    printf("a usart transmit test example!");

    /* 配置摄像头 */
    mt9v03x_init();		

//    timer_interrupt_init(TIMER2, 1000);			
//  	HCI_init();
//	  HCI_handle();		

//    TFT_SPI_Init();


 	delay_1ms(1000);

//    TFT_Init(0);

//    TFTSPI_CLS(0xffff);

//    TFTSPI_Fill_Area(0xffff);


    while(1)
    {
        /* 如果摄像头图像采集完毕 */
        if(mt9v03x_finish_flag)
        {
//            /* 发送摄像头图像 */
//            mt9v03x_oled_display();
//            flame_cnt++;
//            OLED_ShowNum(0, 0, flame_cnt, 3, 16);
            mt9v03x_uart_display();



            /* 图像采集完成标志位置0 */
            mt9v03x_finish_flag = 0;

        }

//    TFT_Init(2);




    }
}

/* 重定向printf函数 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}
