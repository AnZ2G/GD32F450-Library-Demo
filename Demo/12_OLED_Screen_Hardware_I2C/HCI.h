#ifndef _HCI_H_
#define _HCI_H_

#include "stdint.h"
#include "systick.h"
#include "gd32f4xx.h"

void HCI_init(void);  //人机交互初始化
void led_key_init(void);  //LED和按键初始化函数
void switch_init(void);  //拨码开关初始化函数
void value_init(void);  //标志位、变量初始化
void HCI_handle(void);  //人机交互主程序
void data_flash_read(void);  //读取flash中的数据
void data_flash_write(void);  //向flash中写入数据

void select_display(unsigned char page,unsigned char x,unsigned char y,unsigned char reverse);  //选择显示界面处理
void set1_up_process(void);  //选择参数上键处理程序
void set1_down_process(void);  //选择参数下键处理程序
void set1_left_process(void);  //选择参数左键处理程序
void set1_right_process(void);  //选择参数右键处理程序
void set1_ld_process(void);  //选择参数左下键处理程序
void set1_rd_process(void);  //选择参数右下键处理程序

void data_split(unsigned char page,unsigned char x,unsigned char y);//更新value数组中的数值
void renew_data(unsigned char x,unsigned char y);//更新数据

void position_display(unsigned char page,unsigned char x,unsigned char y,unsigned char position,unsigned char reverse);  //参数修改界面处理
void set2_up_process(void);  //参数修改上键处理程序
void set2_down_process(void);  //参数修改下键处理程序
void set2_left_process(void);  //参数修改左键处理程序
void set2_right_process(void);  //参数修改右键处理程序
void set2_ld_process(void);  //参数修改左下键处理程序
void set2_rd_process(void);  //参数修改右下键处理程序

void display_screen(unsigned char screen_num);  //主界面显示处理

void show_data(unsigned char page, unsigned char column, unsigned char reverse, uint32_t data);
void Data_Split(unsigned int uiData, unsigned char ucSize, unsigned char *dp,unsigned char asc);
int Data_Nsplit(unsigned char ucSize, unsigned char *dp);

/*****************第二界面参数********************/
extern uint32_t mid_speed;  //中等速度
extern uint32_t high_speed;  //高速
extern uint32_t low_speed;  //低速
extern uint32_t s_a_value;  //差速的a值
extern uint32_t s_b_value;  //差速的b值
extern uint32_t s_c_value;  //差速的c值
/****************************************************/

/*****************第一界面参数********************/
extern uint32_t P_value;  //电机PID的P值
extern uint32_t I_value;  //电机PID的I值
extern uint32_t D_value;  //电机PID的D值
extern uint32_t PD_P;  //舵机PD的P值
extern uint32_t b_value;  //舵机变P的二次函数中的b
extern uint32_t PD_D;  //舵机PD的D值
/****************************************************/


/*********************预留参数***********************/
extern uint32_t Set_speed_pwm; 
extern uint32_t res2;  
extern uint32_t res3;
extern uint32_t res4;
extern uint32_t res5;
extern uint32_t res6;

#endif

