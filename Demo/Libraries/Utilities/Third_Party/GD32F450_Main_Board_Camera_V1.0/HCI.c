/*****************************************************************************************
文件名称：  210331_1650_HCI.c
项目名称：  人机交互.c文件			 
版 本：     V1.0
说 明：     无
修改记录：  无（初版）
*****************************************************************************************/
#include "stdint.h"
#include "systick.h"
#include "gd32f4xx.h"
#include "wh_flash.h"
#include "oled.h"
#include "HCI.h"
#include "common.h"
#include "headfile.h"
//#include "debug.h"
uint8_t statue_flag;  //状态标志位
uint8_t screen_num;  //界面标号
uint8_t position1_x;  //设定界面1光标x坐标
uint8_t position1_y;  //设定界面1光标y坐标
uint8_t position;  //设定界面2光标的位置
uint8_t move_flag;  //光标移动标志位

/*****************参数类型待确定********************/
extern uint32_t mid_speed;  //中等速度
uint32_t high_speed=20;  //高速
uint32_t low_speed=10;  //低速

uint32_t s_a_value;  //差速的a值
uint32_t s_b_value;  //差速的b值
uint32_t s_c_value;  //差速的c值
/****************************************************/

uint32_t P_value = 5000;  //电机PID的P值
uint32_t I_value = 100;  //电机PID的I值
uint32_t D_value = 0;  //电机PID的D值

uint32_t PD_P=620;  //舵机变P的二次函数中的a
uint32_t b_value=0;  //舵机变P的二次函数中的b
uint32_t PD_D=200;  //舵机PD的D值

uint32_t Set_speed_pwm=130;  //设置速度
uint32_t res2=100;    //定阈值 负责停车检测 防止冲出赛道

/*********************预留参数***********************/
uint32_t res3=3;
uint32_t res4;
uint32_t res5;
uint32_t res6;
uint8_t value[6] = {'\0'};  //存放正在修改的位数为5的变量
uint8_t speed_value[4] = {'\0'};  //存放正在修改的位数为3的变量
uint32_t temp_value;//暂存数值

//用于暂存原来的数据
//暂存电机PID值
uint32_t uiTempPID_P; 
uint32_t uiTempPID_I; 
uint32_t uiTempPID_D;
//暂存舵机PD值
uint32_t uiTempPD_a;
uint32_t uiTempPD_b;
uint32_t uiTempPD_D;
//暂存中高低速参数
uint32_t uiTempSpeed; 
uint32_t uiTempMaxSpeed; 
uint32_t uiTempMinSpeed; 
//暂存差速参数
uint32_t uiTempS_A; 
uint32_t uiTempS_B; 
uint32_t uiTempS_C;
//暂存阈值参数
uint32_t uiTempres1;
uint32_t uiTempres2;
//预留参数
uint32_t uiTempres3;
uint32_t uiTempres4;
uint32_t uiTempres5;
uint32_t uiTempres6;

//用于存入FLASH的数据
unsigned char uc_record_P_value[8]={0};
unsigned char uc_record_I_value[8];
unsigned char uc_record_D_value[8];
unsigned char uc_record_PD_P[8];
unsigned char uc_record_b_value[8];
unsigned char uc_record_PD_D[8];
unsigned char uc_record_mid_speed[8];
unsigned char uc_record_high_speed[8];
unsigned char uc_record_low_speed[8];
unsigned char uc_record_s_a_value[8];
unsigned char uc_record_s_b_value[8];
unsigned char uc_record_s_c_value[8];
unsigned char uc_record_Set_speed_pwm[8];
unsigned char uc_record_res2[8];
unsigned char uc_record_res3[8];
unsigned char uc_record_res4[8];
unsigned char uc_record_res5[8];
unsigned char uc_record_res6[8];

uint8_t write_buffer[144] = {0,};
uint8_t read_buffer[144] = {0,};
uint16_t ms_cnt;

uint16_t x = 1400;
extern uint8_t BI_value;

/*************************************************
函数名称: led_key_init()
函数功能: LED和按键初始化函数（调试用）
调用函数: 无
入口参数: 无
出口参数: 无
返回值:   无
其他:     LED所用管脚号在VCAN_LED.c文件更改
***********************************************/
void led_key_init(void)  //LED和按键初始化函数
{
//    LED_Init();  //初始化所有LED
    KEY_Init(KEY_ALL);  //初始化所有按键
//	  switch_init();
}
/*************************************************
函数名称: switch_init()
函数功能: 拨码开关初始化函数（调试用）
调用函数: 无
入口参数: 无
出口参数: 无
返回值:   无
其他:     拨码开关所用管脚号在HCI.h文件中更改
***********************************************/
void switch_init(void)
{
     /* 设置上拉输入 */
//   	PIN_InitConfig(DIP1_PIN,  PIN_MODE_INPUT_PULLUP, 1);
//	PIN_InitConfig(DIP2_PIN,  PIN_MODE_INPUT_PULLUP, 1);
//	PIN_InitConfig(DIP3_PIN,  PIN_MODE_INPUT_PULLUP, 1);
//	PIN_InitConfig(DIP4_PIN,  PIN_MODE_INPUT_PULLUP, 1);
}
/*************************************************
函数名称: value_init();  
函数功能: 标志位、变量初始化
调用函数: 无
入口参数: 无
出口参数: 无
返回值:   无
其他:     
***********************************************/
void value_init(void)  //标志位、变量初始化
{
    statue_flag = 0;  //状态标志位
    screen_num = 0;  //界面标号
    position1_x = 0;  //设定界面1光标x坐标
    position1_y = 0;  //设定界面1光标y坐标
    position = 0;  //设定界面2光标的位置
    move_flag = 0;  //光标移动标志位
}
/*************************************************
 函数名称: HCI_init()
 函数功能: 人机交互初始化
 调用函数: 无
 入口参数: 无
 出口参数: 无
 返回值:   无
 其他:     无
*************************************************/
void HCI_init(void)
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
    mt9v03x_init();		//配置摄像头
    value_init(); 

    Encoder_Init(Encoder_ALL, Mode2);
}

void HCI_handle(void)
{ 
   data_flash_read();//读取flash中的数据
    
		//暂存电机PID值
    uiTempPID_P = P_value; 
    uiTempPID_I = I_value; 
    uiTempPID_D = D_value;
    //暂存舵机PD值
    uiTempPD_a = PD_P;
    uiTempPD_b = b_value;
    uiTempPD_D = PD_D;
    //暂存中高低速参数
    uiTempSpeed = mid_speed; 
    uiTempMaxSpeed = high_speed; 
    uiTempMinSpeed = low_speed; 
    //暂存差速参数
    uiTempS_A = s_a_value; 
    uiTempS_B = s_b_value; 
    uiTempS_C = s_c_value;

    //预留参数
    uiTempres1 = Set_speed_pwm;
    uiTempres2 = res2; 
    uiTempres3 = res3;
    uiTempres4 = res4;
    uiTempres5 = res5;
    uiTempres6 = res6;
    
    while(1)
    {    
        display_screen(screen_num);  //界面显示函数 
        //切换界面键处理(上下键)
        if( 1 == KEY_Read(1))  //带延时消抖
        {
            clr_Screen(6,44,84);
            print_str8x16(6,44,1,"Shift");//反白显示shift键
            delay_1ms(50);
            Clr_Screen(8);    
            
            if(5 == screen_num)  //5个主界面循环显示
            {
                screen_num = 0;
            }//end of if(3 == screen_num)
            else
            {
                screen_num++;
            }//end of else
        }//end of if( 0 == key_check(KEY_U))切换显示界面
        
        else if( 4 == KEY_Read(1))  //带延时消抖
        {

            clr_Screen(6,44,84);
            print_str8x16(6,44,1,"Shift");//反白显示shift键
            delay_1ms(50);
            Clr_Screen(8);    
            
            if(0 == screen_num)  //4个主界面循环显示
            {
                screen_num = 5;
            }//end of if(3 == screen_num)
            else
            {
                screen_num--;
            }//end of else
        }
    
        //设定键处理
        else if(5 == KEY_Read(1))
        {
            clr_Screen(6,0,18);
            print_str8x16(6,0,1,"Set");  //反白显示Set
            delay_1ms(50);
        
            statue_flag = 1;//进入参数选择显示界面
            
            clr_Screen(6,0,128);
            OLED_ShowString(0,6,"Set",16);  //显示Set
						OLED_ShowString(96,6,"Exit",16);//显示Exit
            select_display(screen_num,position1_x,position1_y,1);  //反白显示主界面0光标所在位置的参数
            
            while(0 != statue_flag)
            {
          
                while(1 == statue_flag)
                {
                    //选择显示处理
                    if(1 == move_flag)//如果光标移动了则更新显示
                    {
                        move_flag = 0;
                        select_display(screen_num,position1_x,position1_y,1);//更新显示移动后光标的位置
                    }//end of if(1 == statue_flag)
                    
                    if(1 == KEY_Read(1))
                    {
                        select_display(screen_num,position1_x,position1_y,0);//清除移动前光标的反白
                        set1_up_process();                        
                    }//end of if(0 == key_check(KEY_U))
                    
                    else if(4 == KEY_Read(1))
                    {
                        select_display(screen_num,position1_x,position1_y,0);//清除移动前光标的反白
                        set1_down_process();
                    }//end of else if(0 == key_check(KEY_D))
                    
                    else if(2 == KEY_Read(1))
                    {
                        select_display(screen_num,position1_x,position1_y,0);//清除移动前光标的反白
                        set1_left_process();
                    }//end of else if(0 == key_check(KEY_L))
                    
                    else if(3 == KEY_Read(1))
                    {
                        select_display(screen_num,position1_x,position1_y,0);//清除移动前光标的反白
                        set1_right_process();
                    }//end of else if(0 == key_check(KEY_R))
           
                    //设定或退出处理
                    if(5 == KEY_Read(1))
                    {
                        set1_ld_process();
                    }//end of if(0 == key_check(KEY_R))
                    
                    else if(6 == KEY_Read(1))
                    {                  
                        set1_rd_process();
                    }//end of else if(0 == key_check(KEY_RD))
                
                }//end of while(1 == statue_flag)
                if(1 == screen_num)
                {
                    print_str8x16(0,40,0,".");
                    print_str8x16(2,40,0,".");
                    print_str8x16(4,40,0,".");
                }
                //具体参数设定处理
                while(2 == statue_flag)
                {
                    if(1 == move_flag)  //如果光标移动了则更新显示
                    {
                        renew_data(position1_x,position1_y);  //更新数据
                        move_flag = 0;
                        position_display(screen_num,position1_x,position1_y,position,1);
                    }//end of if(1 == move_flag)
                    
                    if(1 == KEY_Read(1))
                    {
                        set2_up_process();
                    }//end of if(0 == key_check(KEY_U))
                    
                    else if(4 == KEY_Read(1))
                    {
                        set2_down_process();
                    }//end of else if(0 == key_check(KEY_D))
                    
                    else if(2 == KEY_Read(1))
                    {
                        position_display(screen_num,position1_x,position1_y,position,0);
                        set2_left_process();
                    }//end of else if(0 == key_check(KEY_L))
                    
                    else if(3 == KEY_Read(1))
                    {
                        position_display(screen_num,position1_x,position1_y,position,0);
                        set2_right_process();
                    }//end of else if(0 == key_check(KEY_R))
            
                    if(5 == KEY_Read(1))
                    {
                        set2_ld_process();
                    }//end of else if(0 == key_check(KEY_LD))
                    
                    else if(6 == KEY_Read(1))
                    {
                         set2_rd_process();
                    }//end of else if(0 == key_check(KEY_RD))
                    
                }//end of while(2 == statue_flag)
            }//end of while(0 != statue_flag)
        }//end of else if(0 == key_check(KEY_R))设定键处理
    
        //起跑键设定
        else if(6 == KEY_Read(1))
        {
            clr_Screen(6,88,112);
            print_str8x16(6,88,1,"Run");//反白显示Run键
            delay_1ms(50);
            Clr_Screen(8);
            break;
        }//end of else if(0 == key_check(KEY_RD))
        else if(7 == KEY_Read(1))
        {
            clr_Screen(6,88,112);
            print_str8x16(6,88,1,"Run");//反白显示Run键
            delay_1ms(50);
            Clr_Screen(8);
            break;
        }//end of else if(0 == key_check(KEY_RD))
    }//end of while(1)  
    
   data_flash_read();  //获取人机交互最新的数据
    
/************************************************************************/    
//    RS_380.kp_value = P_value*0.01;  //用于将人机交互中的参数转换为自己的用到的
//    RS_380.ti_value = I_value*0.01;
//    RS_380.td_value = D_value*0.01;

//		KD = PD_D * 0.01;
//		J = b_value * 0.01;
//		BasicP= PD_P*0.01;
//		motor_l.set_speed = Set_speed_pwm;  //设定速度 原void motor_error(void)***********************************************************************
//		motor_r.set_speed = Set_speed_pwm;  ;
//    RS_380.set_speed = mid_speed * 2.857;//oled上显示的速度转换成编码器数值
//    h_speed = high_speed * 2.857;//直道最高速度
//    l_speed = low_speed * 2.857;//弯道减速
/***********************************************************************/     
}     
        

void data_flash_read(void)  //读取flash中的数据
{
		flash_ReadSector_uint8_t(20, 0, read_buffer, 144);

		flash_buffer_translate(read_buffer, uc_record_P_value, 0, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_I_value, 8, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_D_value, 16, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_PD_P, 24, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_b_value, 32, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_PD_D, 40, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_mid_speed, 48, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_high_speed, 56, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_low_speed, 64, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_s_a_value, 72, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_s_b_value, 80, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_s_c_value, 88, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_Set_speed_pwm, 96, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_res2, 104, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_res3, 112, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_res4, 120, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_res5, 128, 0, 8);
		flash_buffer_translate(read_buffer, uc_record_res6, 136, 0, 8);

		P_value= Data_Nsplit(8,uc_record_P_value);
		I_value = Data_Nsplit(8,uc_record_I_value);
		D_value =	Data_Nsplit(8,uc_record_D_value);
		PD_P = Data_Nsplit(8,uc_record_PD_P);
		b_value = Data_Nsplit(8,uc_record_b_value);
		PD_D = Data_Nsplit(8,uc_record_PD_D);
		mid_speed = Data_Nsplit(8,uc_record_mid_speed);
		high_speed = Data_Nsplit(8,uc_record_high_speed);
		low_speed = Data_Nsplit(8,uc_record_low_speed);
		s_a_value =	Data_Nsplit(8,uc_record_s_a_value);
		s_b_value =	Data_Nsplit(8,uc_record_s_b_value);
		s_c_value =	Data_Nsplit(8,uc_record_s_c_value);
		Set_speed_pwm =	Data_Nsplit(8,uc_record_Set_speed_pwm);
		res2 = Data_Nsplit(8,uc_record_res2);
		res3 = Data_Nsplit(8,uc_record_res3);
		res4 = Data_Nsplit(8,uc_record_res4);
		res5 = Data_Nsplit(8,uc_record_res5);     
		res6 = Data_Nsplit(8,uc_record_res6);     		
}
  

void data_flash_write(void)//向flash中写入数据
{
			Data_Split(P_value, 8, uc_record_P_value,0);
			Data_Split(I_value, 8, uc_record_I_value,0);
			Data_Split(D_value, 8, uc_record_D_value,0);
			Data_Split(PD_P, 8, uc_record_PD_P,0);
			Data_Split(b_value, 8, uc_record_b_value,0);
			Data_Split(PD_D, 8, uc_record_PD_D,0);
			Data_Split(mid_speed, 8, uc_record_mid_speed,0);
			Data_Split(high_speed, 8, uc_record_high_speed,0);
			Data_Split(low_speed, 8, uc_record_low_speed,0);
			Data_Split(s_a_value, 8, uc_record_s_a_value,0);
			Data_Split(s_b_value, 8, uc_record_s_b_value,0);
			Data_Split(s_c_value, 8, uc_record_s_c_value,0);
			Data_Split(Set_speed_pwm, 8, uc_record_Set_speed_pwm,0);
			Data_Split(res2, 8, uc_record_res2,0);
			Data_Split(res3, 8, uc_record_res3,0);
			Data_Split(res4, 8, uc_record_res4,0);
			Data_Split(res5, 8, uc_record_res5,0);
			Data_Split(res6, 8, uc_record_res6,0);

			flash_buffer_translate(uc_record_P_value, write_buffer, 0, 0, 8);
			flash_buffer_translate(uc_record_I_value, write_buffer, 0, 8, 8);
			flash_buffer_translate(uc_record_D_value, write_buffer, 0, 16, 8);
			flash_buffer_translate(uc_record_PD_P, write_buffer, 0, 24, 8);
			flash_buffer_translate(uc_record_b_value, write_buffer, 0, 32, 8);
			flash_buffer_translate(uc_record_PD_D, write_buffer, 0, 40, 8);
			flash_buffer_translate(uc_record_mid_speed, write_buffer, 0, 48, 8);
			flash_buffer_translate(uc_record_high_speed, write_buffer, 0, 56, 8);
			flash_buffer_translate(uc_record_low_speed, write_buffer, 0, 64, 8);
			flash_buffer_translate(uc_record_s_a_value, write_buffer, 0, 72, 8);
			flash_buffer_translate(uc_record_s_b_value, write_buffer, 0, 80, 8);
			flash_buffer_translate(uc_record_s_c_value, write_buffer, 0, 88, 8);
			flash_buffer_translate(uc_record_Set_speed_pwm, write_buffer, 0, 96, 8);
			flash_buffer_translate(uc_record_res2, write_buffer, 0, 104, 8);
			flash_buffer_translate(uc_record_res3, write_buffer, 0, 112, 8);
			flash_buffer_translate(uc_record_res4, write_buffer, 0, 120, 8);
			flash_buffer_translate(uc_record_res5, write_buffer, 0, 128, 8);
			flash_buffer_translate(uc_record_res6, write_buffer, 0, 136, 8);

			flash_WriteSector_uint8_t(20, 0, write_buffer, 144);

}
  
void select_display(unsigned char page,unsigned char x,unsigned char y,unsigned char reverse)//参数选择界面显示程序
{
    if(0 == page)//主界面1
    {
        if( (0 == x)&&(0 == y) )  //反白显示PID的P值
        {
            clr_Screen(0,0,56);
            print_str8x16(0,0,reverse,"P:");
            show_data(0,16,reverse,P_value);
        }//end of if( (0 == x)&&(0 == y) )
        else if( (0 == x)&&(1 == y) )  //反白显示PID的I值
        {
            clr_Screen(2,0,56);
            print_str8x16(2,0,reverse,"I:");
            show_data(2,16,reverse,I_value);
        }//end of else if( (0 == x)&&(1 == y) )
        else if( (0 == x)&&(2 == y) )  //反白显示PID的D值
        {
            clr_Screen(4,0,56);
            print_str8x16(4,0,reverse,"D:");
            show_data(4,16,reverse,D_value);
        }//end of else if( (0 == x)&&(2 == y) )
        else if( (1 == x)&&(0 == y) )  //反白显示变P的a
        {
            clr_Screen(0,72,128);
            print_str8x16(0,72,reverse,"P:");//72=7*8+2*8
            show_data(0,88,reverse,PD_P);//88=72+2*8
        }//end of else if( (1 == x)&&(0 == y) )
        else if( (1 == x)&&(1 == y) )  //反白显示变P的b
        {
            clr_Screen(2,56,128);
            print_str8x16(2,72,reverse,"b:");
            show_data(2,88,reverse,b_value);
        }//end of else if( (1 == x)&&(1 == y) )
        else if( (1 == x)&&(2 == y) )  //反白显示PD的D值
        {
            clr_Screen(4,56,128);
            print_str8x16(4,72,reverse,"D:");
            show_data(4,88,reverse,PD_D);
        }//end of else if( (1 == x)&&(2 == y) )
    }
    else if(1 == page)
    {
        if( (0 == x)&&(0 == y) )  //反白显示中等速度
        {
            clr_Screen(0,0,64);
            print_str8x16(0,0,reverse,"M_S:");
            show_data(0,32,reverse,mid_speed);  //显示小数函数
        }//end of if( (0 == x)&&(0 == y) )
        else if( (0 == x)&&(1 == y) )  //反白显示高速
        {
            clr_Screen(2,0,64);
            print_str8x16(2,0,reverse,"H_S:");
            show_data(2,32,reverse,high_speed);
        }//end of else if( (0 == x)&&(1 == y) )
        else if( (0 == x)&&(2 == y) )  //反白显示低速
        {
            clr_Screen(4,0,64);
            print_str8x16(4,0,reverse,"L_S:");
            show_data(4,32,reverse,low_speed);
        }//end of else if( (0 == x)&&(2 == y) )
        else if( (1 == x)&&(0 == y) )  //反白显示差速的a值
        {
            clr_Screen(0,64,128);
            print_str8x16(0,72,reverse,"a:");//72=7*8+2*8
            show_data(0,88,reverse,s_a_value);//88=72+2*8
        }//end of else if( (1 == x)&&(0 == y) )
        else if( (1 == x)&&(1 == y) )  //反白显示差速的b值
        {
            clr_Screen(2,64,128);
            print_str8x16(2,72,reverse,"b:");
            show_data(2,88,reverse,s_b_value);
        }//end of else if( (1 == x)&&(1 == y) )
        else if( (1 == x)&&(2 == y) )  //反白显示差速的c值
        {
            clr_Screen(4,64,128);
            print_str8x16(4,72,reverse,"c:");
            show_data(4,88,reverse,s_c_value);
        }//end of else if( (1 == x)&&(2 == y) )
    }
    else if(2 == page)
    {
        if( (0 == x)&&(0 == y) )  //反白显示预留参数1（根据需要自己修改）
        {
            clr_Screen(0,0,56);
            print_str8x16(0,0,reverse,"1:");
            show_data(0,16,reverse,Set_speed_pwm);
        }//end of if( (0 == x)&&(0 == y) )
        else if( (0 == x)&&(1 == y) )  //反白显示预留参数2（根据需要自己修改）
        {
            clr_Screen(2,0,56);
            print_str8x16(2,0,reverse,"2:");
            show_data(2,16,reverse,res2);
        }//end of else if( (0 == x)&&(1 == y) )
        else if( (0 == x)&&(2 == y) )  //反白显示预留参数3（根据需要自己修改）
        {
            clr_Screen(4,0,56);
            print_str8x16(4,0,reverse,"3:");
            show_data(4,16,reverse,res3);
        }//end of else if( (0 == x)&&(2 == y) )
        else if( (1 == x)&&(0 == y) )  //反白显示预留参数4（根据需要自己修改）
        {
            clr_Screen(0,72,128);
            print_str8x16(0,72,reverse,"4:");//72=7*8+2*8
            show_data(0,88,reverse,res4);//88=72+2*8
        }//end of else if( (1 == x)&&(0 == y) )
        else if( (1 == x)&&(1 == y) )  //反白显示预留参数5（根据需要自己修改）
        {
            clr_Screen(2,56,128);
            print_str8x16(2,72,reverse,"5:");
            show_data(2,88,reverse,res5);
        }//end of else if( (1 == x)&&(1 == y) )
        else if( (1 == x)&&(2 == y) )  //反白显示预留参数6（根据需要自己修改）
        {
            clr_Screen(4,56,128);
            print_str8x16(4,72,reverse,"6:");
            show_data(4,88,reverse,res6);
        }//end of else if( (1 == x)&&(2 == y) )
    }
}


void set1_up_process(void)//参数选择界面上键处理程序
{
    move_flag = 1;
    
    if(0 == position1_y)
    {
        position1_y = 2;
    }
    else
    {
        position1_y--;
    }
}
    

void set1_down_process(void)//参数选择界面下键处理程序
{ 
    move_flag = 1;
    
    if(2 == position1_y)
    {
        position1_y = 0;
    }
    else
    {
        position1_y++;
    }
}


void set1_left_process(void)//参数选择界面左键处理程序
{
    move_flag = 1;
    if(0 == position1_x)
    {
        position1_x = 1;
    }
    else
    {
        position1_x--;
    }
}
    

void set1_right_process(void)//参数选择界面右键处理程序
{ 
    move_flag = 1;
    if(1 == position1_x)
    {
        position1_x = 0;
    }
    else
    {
        position1_x++;
    }
}


void set1_ld_process(void)//参数选择界面左下键处理程序
{   
    clr_Screen(6,0,17);  //反白显示Set
    print_str8x16(6,0,1,"Set");
    delay_1ms(50);
    
    statue_flag = 3;
    
    data_split(screen_num,position1_x,position1_y);

    if((1 == screen_num)&&(0 == position1_x))
    {
        temp_value = speed_value[0] + 10*speed_value[2];
    }
    else
    {
        temp_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
    }
    
    //切换到具体参数设定处理界面
    clr_Screen(6,0,128);  //具体参数设定处理
    print_str8x16(6,0,0,"Ok");  //显示Ok
    print_str8x16(6,96,0,"Back");  //显示Back
    select_display(screen_num,position1_x,position1_y,0);  //清光标位置的反白
    position_display(screen_num,position1_x,position1_y,position,1);  //反白显示待设定参数的待设定位
    statue_flag = 2;    
}


void set1_rd_process(void)//参数选择界面右下键处理程序
{
    unsigned char renew_flag;
    renew_flag = 0;
    
    clr_Screen(6,64,128);  //反白显示Exit
    print_str8x16(6,96,1,"Exit");
    delay_1ms(50);
    
    statue_flag = 0;  //返回到主界面显示
    position1_x = 0;
    position1_y = 0;
    Clr_Screen(8);
    if(P_value != uiTempPID_P)
    {
        renew_flag = 1;
    }
    else if(I_value != uiTempPID_I)
    {
        renew_flag = 1;
    }
    else if(D_value != uiTempPID_D)
    {
        renew_flag = 1;
    }
    else if(PD_P != uiTempPD_a)
    {
        renew_flag = 1;
    }
    else if(b_value != uiTempPD_b)
    {
        renew_flag = 1;
    }
    else if(PD_D != uiTempPD_D)
    {
        renew_flag = 1;
    }
    else if(mid_speed != uiTempSpeed)
    {
        renew_flag = 1;
    }
    else if(high_speed != uiTempMaxSpeed)
    {
        renew_flag = 1;
    }
    else if(low_speed != uiTempMinSpeed)
    {
        renew_flag = 1;
    }
    else if(s_a_value != uiTempS_A)
    {
        renew_flag = 1;
    }
    else if(s_b_value != uiTempS_B)
    {
        renew_flag = 1;
    }
    else if(s_c_value != uiTempS_C)
    {
        renew_flag = 1;
    }
    else if(Set_speed_pwm != uiTempres1)
    {
        renew_flag = 1;
    }
    else if(res2 != uiTempres2)
    {
        renew_flag = 1;
    }
    else if(res3 != uiTempres3)
    {
        renew_flag = 1;
    }
    else if(res4 != uiTempres4)
    {
        renew_flag = 1;
    }    
    else if(res5 != uiTempres5)
    {
        renew_flag = 1;
    }    
    else if(res6 != uiTempres6)
    {
        P_value = 500;
        I_value = 10;
        D_value = 10;
        PD_P = 5;
        b_value = 0;
        PD_D = 0;
        mid_speed = 40;
        high_speed = 0;
        low_speed = 0;
        s_a_value = 0;
        s_b_value = 0;
        s_c_value = 0;
        Set_speed_pwm = 0;
        res2 = 0;
        res3 = 0;
        res4 = 0;
        res5 = 0;
        res6 = 1;
        renew_flag = 1;
    }    
    
    if(1  == renew_flag)
    {
        renew_flag = 0;
        data_flash_write();//向flash中写入数据
    }//end of if(1 == renew_flag)
}


void data_split(unsigned char page,unsigned char x,unsigned char y)//更新value数组中的数值
{
    unsigned char i;
    if(0 == page)
    {
        if( (0 == x)&&(0 == y) )
        {
            Data_Split(P_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(1 == y) )
        {
            Data_Split(I_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(2 == y) )
        {
            Data_Split(D_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(0 == y) )
        {
            Data_Split(PD_P,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(1 == y) )
        {
            Data_Split(b_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(2 == y) )
        {
            Data_Split(PD_D,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
                
        i = value[4];//Data_Split()拆分完的数组的第0个元素是最高位因此位置要调换一下
        value[4] = value[0];
        value[0] = i;
        i = value[3];
        value[3] = value[1];
        value[1] = i;
    }
    else if(1 == page)
    {
        if( (0 == x)&&(0 == y) )
        {
            Data_Split(mid_speed,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(1 == y) )
        {
            Data_Split(high_speed,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(2 == y) )
        {
            Data_Split(low_speed,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(0 == y) )
        {
            Data_Split(s_a_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(1 == y) )
        {
            Data_Split(s_b_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(2 == y) )
        {
            Data_Split(s_c_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        i = value[4];//Data_Split()拆分完的数组的第0个元素是最高位因此位置要调换一下
        value[4] = value[0];
        value[0] = i;
        i = value[3];
        value[3] = value[1];
        value[1] = i;
        if(0 == x)
        {
            speed_value[0] = value[0];
            speed_value[2] = value[1];
        }
    }
    else if(2 == page)
    {
        if( (0 == x)&&(0 == y) )
        {
            Data_Split(Set_speed_pwm,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(1 == y) )
        {
            Data_Split(res2,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        } 
        else if( (0 == x)&&(2 == y) )
        {
            Data_Split(res3,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(0 == y) )
        {
            Data_Split(res4,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(1 == y) )
        {
            Data_Split(res5,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
        else if( (1 == x)&&(2 == y) )
        {
            Data_Split(res6,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
        }
                
        i = value[4];//Data_Split()拆分完的数组的第0个元素是最高位因此位置要调换一下
        value[4] = value[0];
        value[0] = i;
        i = value[3];
        value[3] = value[1];
        value[1] = i;
    }
}


void position_display(unsigned char page,unsigned char x,unsigned char y,unsigned char position,unsigned char reverse)//具体参数设定显示程序
{
    unsigned char column;
    unsigned char begin;
    unsigned char final;

    if(0 == x)//确定光标（反白）的位置
    {
        column = 6 - position;
    }
    else
    {
        column = 15 - position;
    }
        
    begin = 8*column;//清除一个点
    final = begin + 8;
    
    if((1 == page)&&(0 == x))
    {
        speed_value[position] = speed_value[position] + 0x30;
        if(0 == y)  //反白显示PID中P的某一位
        {
            clr_Screen(0,begin,final);
            print_char8x16(0,begin,reverse,&speed_value[position]);
        } 
        else if(1 == y)  //反白显示PID中I的某一位
        {
            clr_Screen(2,begin,final);
            print_char8x16(2,begin,reverse,&speed_value[position]);
        } 
        else if(2 == y)  //反白显示PID中D的某一位
        {
            clr_Screen(4,begin,final);
            print_char8x16(4,begin,reverse,&speed_value[position]);
        }
        speed_value[position] = speed_value[position] - 0x30;
    }//end of if((1 == page)&&(0 == x))
    
    else
    {
        value[position] = value[position] + 0x30;
        if( (0 == x)&&(0 == y) )  //反白显示PID中P的某一位
        {
            clr_Screen(0,begin,final);
            print_char8x16(0,begin,reverse,&value[position]);
        } 
        else if((0 == x)&&(1 == y))  //反白显示PID中I的某一位
        {
            clr_Screen(2,begin,final);
            print_char8x16(2,begin,reverse,&value[position]);
        } 
        else if( (0 == x)&&(2 == y) )  //反白显示PID中D的某一位
        {
            clr_Screen(4,begin,final);
            print_char8x16(4,begin,reverse,&value[position]);
        }
        else if( (1 == x)&&(0 == y) )  //反白显示变P中a的某一位
        {
            clr_Screen(0,begin,final);
            print_char8x16(0,begin,reverse,&value[position]);
        }
        else if( (1 == x)&&(1 == y) )  //反白显示变P中b的某一位
        {
            clr_Screen(2,begin,final);
            print_char8x16(2,begin,reverse,&value[position]);
        }
        else if( (1 == x)&&(2 == y) )  //反白显示PD中D的某一位
        {
            clr_Screen(4,begin,final);
            print_char8x16(4,begin,reverse,&value[position]);
        }    
        value[position] = value[position] - 0x30;
    }//end of else        
}


//参数修改界面各键的处理程序
void set2_up_process(void)//具体参数设定上键处理程序
{
    move_flag = 1;
    if((1 == screen_num)&&(0 == position1_x))
    {
        if(9 == speed_value[position])
        {
            speed_value[position] = 0;
        }
        else
        {
            speed_value[position]++;
        }
    }
    else
    {
        if(9 == value[position])
        {
            value[position] = 0;
        }
        else
        {
            value[position]++;
        }
    }
}
    

void set2_down_process(void)//具体参数设定下键处理程序
{ 
    move_flag = 1;
    if((1 == screen_num)&&(0 == position1_x))
    {
        if(0 == speed_value[position])
        {
            speed_value[position] = 9;
        }
        else
        {
            speed_value[position]--;
        }
    }
    else
    {
        if(0 == value[position])
        {
            value[position] = 9;
        }
        else
        {
            value[position]--;
        }
    }
}


void set2_left_process(void)//具体参数设定左键处理程序
{
    move_flag = 1;
    if((1 == screen_num)&&(0 == position1_x))
    {
        if(0 == position)
        {
            position = 2;
        }
        else
        {
            position = 0;
        }
    }
    else
    {
        if(4 == position)
        {
            position = 0;
        }
        else
        {
            position++;
        }
    }
}
    

void set2_right_process(void)//具体参数设定右键处理程序
{ 
    move_flag = 1;

    if((1 == screen_num)&&(0 == position1_x))
    {
        if(0 == position)
        {
            position = 2;
        }
        else
        {
            position = 0;
        }
    }
    else
    {
        if(0 == position)
        {
            position = 4;
        }
        else
        {
            position--;
        }
    }
}


void renew_data(unsigned char x,unsigned char y)//更新修改后数据
{
    if(0 == screen_num)
    {
        if( (0 == x)&&(0 == y) )
        {
            P_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(0 == y) )
    
        else if( (0 == x)&&(1 == y) )
        {
            I_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(1 == y) )
    
        else if( (0 == x)&&(2 == y) )
        {
            D_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(2 == y) )
    
        else if( (1 == x)&&(0 == y) )
        {
            PD_P = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (1 == x)&&(0 == y) )
    
        else if( (1 == x)&&(1 == y) )
        {
            b_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
        }//end of if( (1 == x)&&(1 == y) )
    
        else if( (1 == x)&&(2 == y) )
        {
            PD_D = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (1 == x)&&(2 == y) )
    }
    else if(1 == screen_num)
    {
        if(0 == x)
        {
            if(0 == y)
            {
                mid_speed = speed_value[0] + 10*speed_value[2];
            }
            else if(1 == y)
            {
                high_speed = speed_value[0] + 10*speed_value[2];
            }
            else if(2 == y)
            {
                low_speed = speed_value[0] + 10*speed_value[2];
            }
        }
        else
        {
            if(0 == y)
            {
                s_a_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
            }
            else if(1 == y)
            {
                s_b_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
            }
            else if(2 == y)
            {
                s_c_value = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
            }          
        }
    }
    else if(2 == screen_num)
    {
        if( (0 == x)&&(0 == y) )
        {
            Set_speed_pwm = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(0 == y) )
    
        else if( (0 == x)&&(1 == y) )
        {
            res2 = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(1 == y) )
    
        else if( (0 == x)&&(2 == y) )
        {
            res3 = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (0 == x)&&(2 == y) )
    
        else if( (1 == x)&&(0 == y) )
        {
            res4 = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (1 == x)&&(0 == y) )
    
        else if( (1 == x)&&(1 == y) )
        {
            res5 = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];
        }//end of if( (1 == x)&&(1 == y) )
    
        else if( (1 == x)&&(2 == y) )
        {
            res6 = value[0]+10*value[1]+100*value[2]+1000*value[3]+10000*value[4];     
        }//end of if( (1 == x)&&(2 == y) )

    }
}


void set2_ld_process(void)//具体参数设定左下键处理程序
{
    clr_Screen(6,0,16);
    print_str8x16(6,0,1,"Ok");  //反白显示显示Ok
    delay_1ms(50);
    
    statue_flag = 1;//返回到参数选择界面
    renew_data(position1_x,position1_y);//更新数据
    select_display(screen_num,position1_x,position1_y,1);
    clr_Screen(6,0,128);
    print_str8x16(6,0,0,"Set");  //显示Set
    print_str8x16(6,96,0,"Exit");  //显示Exit
    position = 0;
}


void set2_rd_process(void)//具体参数设定右下键处理程序
{
    unsigned char i;
    clr_Screen(6,64,128);
    print_str8x16(6,96,1,"Back");  //反白显示显示Back
    delay_1ms(50);
    
    //back按键按下则不保存刚修改的数据
    Data_Split(temp_value,5,value,0);  //拆分为单纯的数字，更新value数组中的数值
    i = value[4];//Data_Split()拆分完的数组的第0个元素是最高位因此位置要调换一下
    value[4] = value[0];
    value[0] = i;
    i = value[3];
    value[3] = value[1];
    value[1] = i;
    speed_value[0] = value[0];
    speed_value[2] = value[1];
    renew_data(position1_x,position1_y);//更新修改后数据
      
    statue_flag = 1;
    select_display(screen_num,position1_x,position1_y,1);
    position = 0;
    clr_Screen(6,0,128);
    print_str8x16(6,0,0,"Set");  //显示Set
    print_str8x16(6,96,0,"Exit");  //显示Exit
}


void display_screen(unsigned char screen_num)//显示主界面处理程序
{
    if(0 == screen_num)  //主界面1显示
    {
        print_str8x16(0,0,0,"P:");
        show_data(0,16,0,P_value);
        print_str8x16(2,0,0,"I:");
        show_data(2,16,0,I_value);
        print_str8x16(4,0,0,"D:");
        show_data(4,16,0,D_value);
      
        print_str8x16(0,72,0,"P:");//72=7*8+2*8
        show_data(0,88,0,PD_P);//88=72+2*8
        print_str8x16(2,72,0,"b:");
        show_data(2,88,0,b_value);
        print_str8x16(4,72,0,"D:");
        show_data(4,88,0,PD_D);
      
        print_str8x16(6,0,0,"Set");
        print_str8x16(6,44,0,"Shift");//Set+14小格
        print_str8x16(6,104,0,"Run");
    }//end of if(0 == screen_num)
    
    else if(1 == screen_num)
    {
        //设定速度参数
        print_str8x16(0,0,0,"M_S:");
        show_data(0,32,0,mid_speed);
        print_str8x16(2,0,0,"H_S:");
        show_data(2,32,0,high_speed);
        print_str8x16(4,0,0,"L_S:");
        show_data(4,32,0,low_speed);
        
        position1_x = 1;//仅仅为了使abc不显示小数
        //差速参数
        print_str8x16(0,72,0,"a:");//72=7*8+2*8
        show_data(0,88,0,s_a_value);//88=72+2*8
        print_str8x16(2,72,0,"b:");
        show_data(2,88,0,s_b_value);
        print_str8x16(4,72,0,"c:");
        show_data(4,88,0,s_c_value);
        position1_x = 0;
        
        print_str8x16(6,0,0,"Set");
        print_str8x16(6,44,0,"Shift");//Set+14小格
        print_str8x16(6,104,0,"Run");
    }//end of if(1 == screen_num)
    
    else if(2 == screen_num)
    {
        //设定阈值参数
        print_str8x16(0,0,0,"1:");
        show_data(0,16,0,Set_speed_pwm);
        print_str8x16(2,0,0,"2:");
        show_data(2,16,0,res2);
        print_str8x16(4,0,0,"3:");
        show_data(4,16,0,res3);
        
        //预留参数
        print_str8x16(0,72,0,"4:");//72=7*8+2*8
        show_data(0,88,0,res4);//88=72+2*8
        print_str8x16(2,72,0,"5:");
        show_data(2,88,0,res5);
        print_str8x16(4,72,0,"6:");
        show_data(4,88,0,res6);
        
        print_str8x16(6,0,0,"Set");
        print_str8x16(6,44,0,"Shift");//Set+14小格
        print_str8x16(6,104,0,"Run");
    }
    
    else if(3 == screen_num)//显示电感参数
    {

        print_str8x16(6,44,0,"Shift");//Set+14小格
        print_str8x16(6,104,0,"Run"); 
        
        print_str8x16(0,24,1,"mid:");    //显示中间电感值
        show_data(0,56,1, 1);
          
        print_str8x16(2,0,1,"L:");    //显示水平左电感值
       show_data(2,16,0,2);
          
        print_str8x16(2,64,1,"R:");    //显示水平右电感值
        show_data(2,80,0, 2);
          
        print_str8x16(4,0,1,"8L:");    //显示8字左电感值
        show_data(4,24,0, 3);  
          
        print_str8x16(4,64,1,"8R:");    //显示8字右电感值
      show_data(4,88,0,4); 
    }//end of else if(3 == screen_num)

    else if(4 == screen_num)//空白页面
    {
        OLED_ShowNum(0,0,timer_counter_read(TIMER2),5,16);
        OLED_ShowNum(0,2,50000-timer_counter_read(TIMER4),5,16);
        if((50000-timer_counter_read(TIMER4)-timer_counter_read(TIMER2))<=0) OLED_ShowNum(0,6,50000-timer_counter_read(TIMER4)-timer_counter_read(TIMER2),6,16);
        else OLED_ShowNum(0,6,-(50000-timer_counter_read(TIMER4)-timer_counter_read(TIMER2)),6,16);
//        motor_l.real_speed = Encoder_Read(Encoder2);
//        motor_r.real_speed = Encoder_Read(Encoder1);


        if(SWITCH_ON == SWITCH_Get_Input(SWITCH1))
        {
            timer_counter_value_config(TIMER2,0);
            timer_counter_value_config(TIMER4,50000);
        } 
//        else if(SWITCH_ON == SWITCH_Get_Input(SWITCH2)) 
//        {
//            PWM_Duty_Updata(0,0);
//            PWM_Duty_Updata(1,300);
//        }
//        else
//        {
//            PWM_Duty_Updata(0,0);
//            PWM_Duty_Updata(1,0);
//        }

//        if(SWITCH_ON == SWITCH_Get_Input(SWITCH3))
//        {
//            PWM_Duty_Updata(2,300);
//            PWM_Duty_Updata(3,0);
//        } 
//        else if(SWITCH_ON == SWITCH_Get_Input(SWITCH4)) 
//        {
//            motor_l.set_speed = 40;
//            motor_r.set_speed = 40;

//        }
//        else
//        {
//            motor_l.set_speed = 0;
//            motor_r.set_speed = 0;
//        }
//        motor_l.real_speed = Encoder_Read(Encoder2);
//        motor_r.real_speed = Encoder_Read(Encoder1);
//        motor_error();
//        motor_control();

        OLED_ShowNum(  0, 4, 10000+x, 5,16 );
        if(KEY_Read(1)==2)
        {
            x=x+10;
        }
        if(KEY_Read(1)==3)
        {
            x=x-10;
        }
        Steer_Updata(x);
    }//end of else if(5 == screen_num)
    else if(5 == screen_num)//空白页面
    {
        if(KEY_PRESS == KEY_Get_Input(KEY_LEFT)) BI_value = BI_value+1;
        else if(KEY_PRESS == KEY_Get_Input(KEY_RIGHT)) BI_value = BI_value-1;

        if(SWITCH_ON == SWITCH_Get_Input(SWITCH1)) Camera_oled_display(BI_value);
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH2)) Camera_tft_display();
        if(SWITCH_ON == SWITCH_Get_Input(SWITCH3)) Camera_uart_display();
        OLED_ShowNum(0,0,BI_value,3,16);
    }//end of else if(4 == screen_num)
}


/***********************************************************************************
* 函数名: show_data;
*
* 描述: 显示变量;
*              
* 入口参数:  unsigned char page, unsigned char column, unsigned char reverse, uint16 data;
             页地址;             列地址;               是否反白显示;          要显示的变量名;
*
* 出口参数: 无;
*
* 备注:  调用函数print_str8x16（）;
************************************************************************************/

void show_data(unsigned char page, unsigned char column, unsigned char reverse, uint32_t data)
{
    unsigned char show_data[6] ={' '};
    unsigned char show_float[4];
    unsigned char display[6];
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char statue =0;
    Data_Split(data,5,show_data,1);
    for(i = 0; i < 6; i++)
    {
        display[i] = ' ';
    }
    if((1 == screen_num)&&(0 == position1_x))
    {
        show_float[3] = show_data[5];
        show_float[2] = show_data[4];
        show_float[1] = '.';
        show_float[0] = show_data[3];
        print_str8x16(page,column,reverse,show_float);
    }
    else
    {
        if(3 == statue_flag)
        {
            print_str8x16(page,column,reverse,show_data);
        }
        else
        {
            for(i = 0; i < 5; i++)
            {
                if( ('0' == show_data[i]) && (0 == statue) )
                {
                    ;
                }
                else
                {
                    statue = 1;
                    display[j] = show_data[i];
                    j++;
                }
            }

            if(0 == j)
            {
                display[0] = '0';
            }
            display[5] = '\0';
            
            print_str8x16(page,column,reverse,display);
        }
    }
    statue = 0;
}


//************拆分数据(暂定最多拆分5位，可调)************
//asc为1时，uidata 拆分为asc码，即0x30~0x39；asc为0时，拆分单纯的数字,即0~9
void Data_Split(unsigned int uiData, unsigned char ucSize, unsigned char *dp,unsigned char asc)
{
     switch(ucSize)
     {
        case 8:
        *(dp++) = (uiData/10000000)%10 + 0x30 * asc;
        case 7:
        *(dp++) = (uiData/1000000)%10 + 0x30 * asc;
        case 6:
        *(dp++) = (uiData/100000)%10 + 0x30 * asc;
			  case 5:
        *(dp++) = (uiData/10000)%10 + 0x30 * asc;
        case 4:
        *(dp++) = (uiData/1000)%10 + 0x30 * asc;
        case 3:
        *(dp++) = (uiData/100)%10 + 0x30 * asc;
        case 2:
        *(dp++) = (uiData/10)%10 + 0x30 * asc;
        case 1:
        *(dp++) = (uiData)%10 + 0x30 * asc;
     }
}
//************拆分数据(暂定最多拆分5位，可调)************
//asc为1时，uidata 拆分为asc码，即0x30~0x39；asc为0时，拆分单纯的数字,即0~9
int Data_Nsplit(unsigned char ucSize, unsigned char *dp)
{
    int num=0; 
		switch(ucSize)
     {
        case 8:
				num=10*num+ *(dp++);
        case 7:
				num=10*num+ *(dp++);
        case 6:
        num=10*num+ *(dp++);
			  case 5:
        num=10*num+ *(dp++);
        case 4:
        num=10*num+ *(dp++);
        case 3:
        num=10*num+ *(dp++);
        case 2:
        num=10*num+ *(dp++);
        case 1:
        num=10*num+ *(dp++);
     }
		 return num;
}

