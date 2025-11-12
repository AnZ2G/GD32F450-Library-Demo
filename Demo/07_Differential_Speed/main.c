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
void find_road(uint8_t value);
uint16_t find_road_control(uint8_t error, uint8_t middle);
uint8_t road_error(void);

uint32_t mid_speed;
uint8_t BI_count=0;
uint16_t steer_value = 1400;
uint8_t HistGram[256];

uint8_t error_buffer[117] = 
{0,0,0,0,0,0,0,0,0,0	//10     
,0,0,0,0,0,0,0,0,0,0	//10
,0,0,0,0,0,0,0,0,0,0	//10
,0,0,0,0,0,0,0,0,0,0	//10
,0,0,0,0,0,0,0,0,0,0	//10
,1,1,1,1,1,1,1,1,1,1	//60
,2,2,2,2,2,2,2,2,2,2	//70
,4,4,4,4,4,4,4,4,4,4	//80
,4,4,4,4,4,4,4,4,4,4	//90
,2,2,2,2,2,2,2,2,2,2	//100
,1,1,1,1,1,1,1,1,1,1	//110
,0,0,0,0,0,0,0};

int main(void)
{
    start_up();
    OLED_OpeningAnimation();
    HCI_handle();
//    delay_1ms(1000);
//    LED_Off(LED1);
//    delay_1ms(1000);
//    LED_Off(LED2);
//    delay_1ms(1000);
//    LED_Off(LED3);
//    delay_1ms(1000);
//    LED_Off(LED4);
//    timer_interrupt_init(TIMER6, 10);
//    timer_interrupt_init(TIMER9, 5000);		
    while(1)
    {
        BI_count++;
        if(BI_count >= 100)
        {
            BI_value = OSTU();
            BI_count = 0;
        }
        BI_error = road_error();
        find_road(BI_error);

        if(BI_value<=130 || SWITCH_ON == SWITCH_Get_Input(SWITCH1)) 
        {
            steer_value = 1400;
            motor_l.set_speed = 0;
            motor_r.set_speed = 0;
        }
        else 
        {
            steer_value = find_road_control(BI_error, 118);
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
//            OLED_ShowNum(0,6,motor_l.set_speed,2,16);
//            OLED_ShowNum(16,6,motor_r.set_speed,2,16);
            flame_cnt = 0;
//            Camera_uart_display_BI();
//            uint8_t i;
//            for(i=MT9V03X_H;i>10;i--)
//            {
//                printf("%d = %d\n",i,middle_line[i]);
//            }
        }

        if(SWITCH_ON == SWITCH_Get_Input(SWITCH4))
        {
//            Camera_oled_display(BI_value);
                uint8_t i;
                flash_buffer_translate(mt9v03x_image[0],Img[0],0,0,MT9V03X_H*MT9V03X_W);
                for(i=116;middle_line[i]!=0;i--)
                {
                    Img[i][middle_line[i]-30] = 0;
                    Img[i][middle_line[i]-29] = 0;
                }
                OLED_camera(BI_value);
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
    Camera_Init();		//配置摄像头
    value_init(); 

    PWM_Config(41,271);
    Encoder_Init(Encoder_ALL, Mode2);
    motor_init();

    BI_value = 140;
}

void find_road(uint8_t value)
{
    uint8_t i,j;
    uint8_t flag1;
    uint8_t flag2;
    uint8_t edge_flag1;
    uint8_t edge_flag2;
    uint8_t temp1=0;
    uint8_t temp2=0;
    uint8_t count=0;
    for(j=0;j<188;j++)
    {
        if(mt9v03x_image[115][j] >= value) 
        {
            temp1 = j;
            break;
        }

    }
    for(j=187;j>0;j--)
    {
        if(mt9v03x_image[115][j] >= value) 
        {
            temp2 = j;
            break;
        }
    }
            OLED_ShowNum(64,6,temp1,3,16);
            OLED_ShowNum(88,6,temp2,3,16);
    for(i=114;i>=10;i--)
    {
        flag1 = 0;
        flag2 = 0;
        edge_flag1 = 0;
        edge_flag2 = 0;
        for(j=temp1;j<=temp1+5;j++)
        {
            if(mt9v03x_image[i][j] >= value) 
            {
                temp1 = j;
                if(j==0) edge_flag1 = 1;
                flag1 = 1;
                break;
            }
        }
        if(flag1==0)
        {
            for(j=temp1;j>=temp1-5;j--)
            {
                if(mt9v03x_image[i][j] >= value) 
                {
                    temp1 = j;
                    if(j==0) edge_flag1 = 1;
                    flag1 = 1;
                    break;
                }
            }
        }
        for(j=temp2;j>=temp2-5;j--)
        {
            if(mt9v03x_image[i][j] >= value) 
            {
                temp2 = j;
                if(j==187) edge_flag2 = 1;
                flag2 = 1;
                break;
            }
        }
        if(flag2==0)
        {
            for(j=temp2;j<=temp2+5;j++)
            {
                if(mt9v03x_image[i][j] >= value) 
                {
                    temp2 = j;
                    if(j==187) edge_flag2 = 1;
                    flag2 = 1;
                    break;
                }
            }
        }
        if(edge_flag1 == 0 && edge_flag2 == 1 && (temp1 + 124)<=255)
        {
            middle_line[i] = temp1 + 30 + 94; 
        }
        else if(edge_flag1 == 1 && edge_flag2 == 0 && (temp2 - 64)>=0)
        {
            middle_line[i] = temp2 + 30 - 94; 
        }
        else if(edge_flag1 == 1 && edge_flag2 == 1)
        {
            middle_line[i] = 124; 
        }
        else if(flag1 != 0 && flag2 != 0 && edge_flag1 == 0 && edge_flag2 == 0) 
        {
            middle_line[i] = (temp1 + temp2)/2 + 30;
        }
    }
} 

uint8_t road_error(void)
{
    uint8_t i;
    uint32_t sum=0;
    uint16_t count=0;
    for(i=114;middle_line[i]!=0;i--)
    {
        sum += middle_line[i] * error_buffer[i];
        count += error_buffer[i];
    }
    return (sum/count);
}

uint16_t find_road_control(uint8_t error, uint8_t middle)
{
    uint16_t steer_value;
//    if(error >= middle-2 && error <= middle+1) steer_value = 1350;
//    else if(error<middle-2 && error>=middle-6) steer_value = (middle-2-error)*5 + 1350;
//    else if(error<middle-6 && error>=middle-18) steer_value = (middle-2-error)*5 + (middle-5-error)*10 + 1350;
//    else if(error<middle-18) steer_value = 16*5 + 13*10 + 1350;
//    else if(error>middle+1 && error<=middle+5) steer_value = 1350 - (BI_error-middle-1)*5;
//    else if(error>middle+5 && error<=middle+17) steer_value = 1350 - (BI_error-middle-1)*5 - (BI_error-middle-4)*10;
//    else if(error>middle+17) steer_value = 1350 - 16*5 - 13*10;

    steer_value = (middle - error)*PD_P + 1400;
    
    if(steer_value>1570) steer_value = 1570;
    else if(steer_value<1230) steer_value = 1230;

    return steer_value;
}
