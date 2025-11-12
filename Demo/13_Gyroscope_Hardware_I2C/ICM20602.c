/*********************************************************************************************************************
* @file				ICM20602.c
* @brief			陀螺仪
* @author			HeFei WeiHuang
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-4
********************************************************************************************************************/

#include "ICM20602.h"
#include "gd32f4xx.h"
#include "wh_i2c.h"
#include "systick.h"
#include <stdio.h>

int16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
int16_t icm_acc_x,icm_acc_y,icm_acc_z;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void Icm20602_Self_Check(void)
{
    uint8_t val;
    do
    {
        i2c_read_buffer(ICM20602_DEV_ADDR, ICM20602_WHO_AM_I, &val ,1);
		//oled_int16_t(0,0,val);
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }while(0x12 != val);
	printf("ICM20602 is ok");
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      软件SPI，初始化ICM20602
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void Icm20602_Init(void)
{
    uint8_t val;

    Icm20602_Self_Check();//检测
    
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1, 0x80);//复位设备
    delay_1ms(2);
    do
    {//等待复位成功
        i2c_read_buffer(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1,&val, 1);
    }while(0x41 != val);
    
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_1,     0x01);            //时钟设置
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_PWR_MGMT_2,     0x00);            //开启陀螺仪和加速度计
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_SMPLRT_DIV,     0x07);            //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_GYRO_CONFIG,    0x18);            //±2000 dps
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG,   0x10);            //±8g
    i2c_send_reg_byte(ICM20602_DEV_ADDR, ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Icm20602_Get_Accdata(void)
{
    uint8_t dat[6];
    
    i2c_read_buffer(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6);
    icm_acc_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    icm_acc_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    icm_acc_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Icm20602_Get_Gyro(void)
{
    uint8_t dat[6];
    
    i2c_read_buffer(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6);
    icm_gyro_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    icm_gyro_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    icm_gyro_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}

