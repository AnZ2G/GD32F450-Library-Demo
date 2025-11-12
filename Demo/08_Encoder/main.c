/*********************************************************************************************************************
* @file				main.c
* @brief			编码器
* @author			HeFei WeiHuang
* @version			2.2
* @Target core		GD32F450ZIT6
* @date				2021-7-7
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板, 展示如何配置TIMER2的正交解码
器和计数功能，从而对编码器输出的脉冲电平进行解码并计数，
通过有线串口将数据发送至电脑。

-在GD32F450学习板上，PB10连接TXD引脚，PB11连接RXD引脚，
PB0、PB5连接编码器输出脚。配置PB0、PB5为复用功能2，作为
TIMER2通道，用于接收编码器输出。使用中间层库函数配置定时器
结构体，并通过底层库函数配置TIMER2的预分频器、重装载、正交
解码器等功能。启动编码器，编码器根据转动圈数输出脉冲信号，
经过解码并计数，通过有线串口发送至电脑。

-打开电源，持续向电脑发送编码器转动情况，间隔1s。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "gd32f450_board.h"
#include "headfile.h"
#include "Encoder.h"
#include "pwm.h"
#include "systick.h"
#include <stdio.h>

void timer_configuration(void);
void timer_config(void);

int main(void)
{
	
    /* 配置滴答计时器 */
    systick_config();

    /* 配置PB10为TXD引脚 */
    uart_tx_init(GPIOB, GPIO_PIN_10, RCU_GPIOB);

    /* 配置PB11为RXD引脚 */
    uart_rx_init(GPIOB, GPIO_PIN_11, RCU_GPIOB);

    /* 配置UART，使用USART2，波特率设置为115200 */
    uart_init(RCU_USART2, USART2, 115200U);    

    printf("a usart transmit test example!");	
		
    /* 配置编码器定时器 */
    Encoder_Init(Encoder_ALL, Mode2);

    /* 配置PWM定时器 */
    pwm_config();

    while(1)
    {
//        pwm_duty_updata(0,200);
//        pwm_duty_updata(1,300);
//        pwm_duty_updata(2,400);
//        pwm_duty_updata(3,500);
        printf("value1 is %d\n",Encoder_Read(Encoder1));
        printf("value2 is %d\n",Encoder_Read(Encoder2));
        delay_1ms(100);
    }
}

/* 重定向printf函数 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}
