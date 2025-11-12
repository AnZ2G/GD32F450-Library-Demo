/********************************************************************************************************************
* @file				key.c
* @author			Andreas WH
* @version			1.0
* @Target core		gd32f450
* @date				2021-8-15
********************************************************************************************************************/

#include "stdlib.h"
#include "string.h" 	
#include "systick.h"
#include "gd32f4xx.h"
#include "wh_gpio.h"
#include "key.h"
#include "debug.h"

/***************************************************************************************
*	函数名称： KEY_Init(void);
*			
*	函数功能： 初始化母版上的按键
*			
*	调用函数： 无
*	    
*	入口参数： 无
*								
* 出口参数： 无
*			
*	返回值： 无
*			
*	备注： 无
*****************************************************************************************/
void KEY_Init(void)
{
	  /* 使能GPIOG时钟 */
    rcu_periph_clock_enable(RCU_GPIOG);
    /* 配置拨码开关引脚 */
    gpio_output_init(KEY_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEY_UP_PIN | KEY_DOWN_PIN | KEY_LEFT_PIN | KEY_RIGHT_PIN | KEY_SET_PIN | KEY_BACK_PIN );
}
/***************************************************************************************
*	函数名称： uint8_t KEY_Read(uint8_t mode);
*			
*	函数功能： 读取按键状态
*			
*	调用函数： 无
*	    
*	入口参数： 无
*								
* 出口参数： 按键键值
*			
*	返回值： 无
*			
*	备注： 无
*****************************************************************************************/
uint8_t KEY_Read(uint8_t mode)
{
	  static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
        key_up=1;      //支持连按
    }
    if(key_up && (gpio_get_input(KEY_GPIO,KEY_UP_PIN)==0 || gpio_get_input(KEY_GPIO,KEY_LEFT_PIN)==0 || gpio_get_input(KEY_GPIO,KEY_RIGHT_PIN)==0|| gpio_get_input(KEY_GPIO,KEY_DOWN_PIN)==0|| gpio_get_input(KEY_GPIO,KEY_SET_PIN)==0|| gpio_get_input(KEY_GPIO,KEY_BACK_PIN)==0))
    {
         delay_1ms(20);  //消抖
        key_up=0;
        if(gpio_get_input(KEY_GPIO,KEY_UP_PIN)==0)      
        {
            return 1;//上
        }
        
        else if(gpio_get_input(KEY_GPIO,KEY_LEFT_PIN)==0) 
        {
            return 2;//左  
        }
        
        else if(gpio_get_input(KEY_GPIO,KEY_RIGHT_PIN)==0) 
        {
            return 3;//右    
        }
				
				else if(gpio_get_input(KEY_GPIO,KEY_DOWN_PIN)==0) 
        {
            return 4; //下   
        }
				
				else if(gpio_get_input(KEY_GPIO,KEY_SET_PIN)==0) 
        {
            return 5; //左下   
        }
				
				else if(gpio_get_input(KEY_GPIO,KEY_BACK_PIN)==0) 
        {
            return 6; //右下  
        }
        
    }
    if((gpio_get_input(KEY_GPIO,KEY_UP_PIN)==1 || gpio_get_input(KEY_GPIO,KEY_LEFT_PIN)==1 || gpio_get_input(KEY_GPIO,KEY_RIGHT_PIN)==1|| gpio_get_input(KEY_GPIO,KEY_DOWN_PIN)==1|| gpio_get_input(KEY_GPIO,KEY_SET_PIN)==1|| gpio_get_input(KEY_GPIO,KEY_BACK_PIN)==1)) 
    {
        key_up=1;   
    }
    return 0;   //无按键按下
    
}

