/*********************************************************************************************************************
* @file				main.c
* @brief			主程序
* @author			HeFei WeiHuang
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-31
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "common.h"
#include "headfile.h"
#include "systick.h"
#include <stdio.h>

extern uint8_t mt9v03x_finish_flag;
uint16_t flame_cnt;
uint8_t BI_value, BI_error;
uint8_t middle_line[MT9V03X_H]={0,};

void start_up(void);
uint8_t find_middle(uint8_t value);
uint8_t find_road(uint8_t value);
uint16_t find_road_control(uint8_t error, uint8_t middle);
uint8_t road_error(void);

uint32_t mid_speed;
uint8_t BI_count=0;
uint16_t steer_value = 1400;
uint8_t HistGram[256];

int main(void)
{
    start_up();
    timer_interrupt_init(TIMER6, 10);
    uint8_t BI_value = 140;
    while(1)
    {
        BI_value = OSTU();
        OLED_ShowNum(0, 0, flame_cnt, 8, 16);
        Camera_oled_display(BI_value);
        flame_cnt = 0;

    }
}

void start_up(void)
{
    systick_config();
    LED_Init(LED_ALL);  //LED和按键初始化函数（调试用）
    KEY_Init(KEY_ALL);
    i2c_config(400000, 0x00);
    OLED_Init();  //OLED初始化
    delay_1ms(200);  //初始化后必要的延时
    Clr_Screen(8);  //清屏

    SWITCH_Init(SWITCH_ALL);  //初始化所有拨码开关
    UART_Init(Wireless_UART, 115200);//配置串口
    TFT_SPI_Init();//配置TFTSPI
    TFT_Init(0);
    TFTSPI_CLS(0xffff);
    Camera_Init();		//配置摄像头
    value_init(); 

    PWM_Config(41,271);
    Encoder_Init(Encoder_ALL, Mode2);
    motor_init();
}
