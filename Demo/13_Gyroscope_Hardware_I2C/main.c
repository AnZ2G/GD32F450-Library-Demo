/*********************************************************************************************************************
* @file				main.c
* @brief			陀螺仪
* @author			HeFei WeiHuang
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-4
********************************************************************************************************************/

/*********************************************************************************************************************

* 例程说明：

-此例程基于GD32F450学习板,  展示如何应用中间层库配置I2C，
应用外部陀螺仪库配置陀螺仪，并将陀螺仪加速度和姿态信息通
过有线串口发送至电脑。

-在GD32F450学习板上，PB10连接TXD引脚，PB11连接RXD引脚，
串口使用USART2，波特率设置为115200。PA8配置为SCL引脚，
PB4配置为SDA引脚。通过外部陀螺仪库函数获取加速度和姿态
信息，并通过有线串口将信息发送至电脑。

-将陀螺仪接入学习板对于位置。打开电源后，通过有线串口
向电脑发送加速度和姿态数据，间隔1s。

********************************************************************************************************************/

#include "gd32f4xx.h"
#include "headfile.h"
#include "gd32f450_board.h"
#include "systick.h"
#include <stdio.h>
#include "ICM20602.h"

extern int16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16_t icm_acc_x,icm_acc_y,icm_acc_z;

int main(void)
{
    /* 配置滴答计时器 */
    systick_config();

    /* 配置LED1、2 */
    led_init(LED1);
    led_init(LED2);

    /* 配置PC10为TXD引脚 */
    uart_tx_init(GPIOB, GPIO_PIN_10, RCU_GPIOB);

    /* 配置PC11为RXD引脚 */
    uart_rx_init(GPIOB, GPIO_PIN_11, RCU_GPIOB);

    /* 配置UART，使用USART2，波特率设置为115200 */
    uart_init(RCU_USART2, USART2, 115200U);    
    
    /* 定义字符串 */
    uint8_t string[] = {"a usart transmit test example!\n"};

    /* 发送字符串 */
    uart_putstr(USART2, string);

    i2c_config(400000, 0x00);

    Icm20602_Init();

    while(1)
    {
        /* 向串口发送陀螺仪姿态数据 */
        delay_1ms(1000);
        Icm20602_Get_Accdata();
        printf("icm_acc_x value is %d \n",icm_acc_x);
        printf("icm_acc_y value is %d \n",icm_acc_y);
        printf("icm_acc_z value is %d \n",icm_acc_z);
				
        /* 向串口发送陀螺仪加速度数据 */
        delay_1ms(1000);
        Icm20602_Get_Gyro();
        printf("icm_gyro_x value is %d \n",icm_gyro_x);
        printf("icm_gyro_y value is %d \n",icm_gyro_y);
        printf("icm_gyro_z value is %d \n",icm_gyro_z);
    }
}

/* 重定向printf函数 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while (RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}
