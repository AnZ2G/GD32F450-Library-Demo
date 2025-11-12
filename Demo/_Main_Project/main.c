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
#include "Image.h"
#include <stdio.h>


extern uint8_t mt9v03x_finish_flag;
uint16_t flame_cnt;
uint8_t BI_value, BI_error;
void start_up(void);
uint8_t find_middle(uint8_t value);
uint8_t find_road(uint8_t value);
uint16_t find_road_control(uint8_t error, uint8_t middle);
uint8_t road_error(void);
uint32_t mid_speed;
uint8_t BI_count=0;
uint16_t steer_value = 1400;
uint8_t HistGram[256];
uint8_t i = 0, j = 0,temp=0,page=0;

int main(void)
{
    start_up();
//    OLED_OpeningAnimation();
    HCI_handle();
    timer_interrupt_init(TIMER6, 10);
    timer_interrupt_init(TIMER9, 5000);		


    while(1)
    {
        if(mt9v03x_finish_flag)
        {
            /* 图像采集完成标志位置0 */
            mt9v03x_finish_flag = 0;  
            ImRez_ori();
            BI_value = my_adapt_threshold_otso(IMG_zip,ColumnMax,RowMax);
//            BI_value = OSTU();
//            if(KEY_PRESS == KEY_Get_Input(KEY_UP)) my_adapt_threshold_otso(IMG_zip,ColumnMax,RowMax);
            Binarization(IMG_zip[0],BI_value);

            MiddleLineSearch2();

            if(SWITCH_ON == SWITCH_Get_Input(SWITCH1))
            {
                Camera_tft_display();
//                Line_Repair();
                FireWater_Send();
                Line_Print();
                Middle_Print();
                OLED_ShowNum(0,0,flame_cnt,5,16);
                OLED_ShowNum(0,2,BI_value,5,16);
                OLED_camera_zip(BI_value);
            }
            flame_cnt = 0;

            BI_error = road_error();

        if(BI_value<=70 || SWITCH_ON == SWITCH_Get_Input(SWITCH3)) 
        {
            steer_value = 1400;
            motor_l.set_speed = 0;
            motor_r.set_speed = 0;
        }
        else 
        {
            steer_value = find_road_control(BI_error, 47);
            motor_l.set_speed = mid_speed;
            motor_r.set_speed = mid_speed;
            stopflag = 1;
        }

        Steer_Updata(steer_value);


        if(SWITCH_ON == SWITCH_Get_Input(SWITCH2))
        {
            OLED_ShowNum(0,2,flame_cnt,6,16);
            OLED_ShowNum(0,0,BI_error,3,16);
            OLED_ShowNum(64,0,steer_value,4,16);
            OLED_ShowNum(0,4,BI_value,3,16);
            OLED_ShowNum(0,6,motor_l.set_speed,2,16);
            OLED_ShowNum(16,6,motor_r.set_speed,2,16);
            flame_cnt = 0;
        }

        }
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
//    Camera_Init();		//配置摄像头
    value_init(); 

    PWM_Config(41,271);
    Encoder_Init(Encoder_ALL, Mode2);
    motor_init();

    BI_value = 140;
}



