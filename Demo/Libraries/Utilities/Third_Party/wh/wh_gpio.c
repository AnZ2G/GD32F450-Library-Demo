/*********************************************************************************************************************
* @file				wh_gpio.c
* @author			Andreas WH
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#include "wh_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO输出初始化
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G,H,I)
// @param		mode				GPIO的模式，只能选择以下参数：
// 	@arg		GPIO_MODE_INPUT:	输入模式
// 	@arg		GPIO_MODE_OUTPUT:	输出模式
// 	@arg		GPIO_MODE_AF:		复用模式
// 	@arg		GPIO_MODE_ANALOG:	模拟模式
// @param		pull_up_down		引脚使用上拉电阻还是下拉电阻，只能选择以下参数：
// 	@arg		GPIO_PUPD_NONE:		浮空模式，不使用电阻
// 	@arg		GPIO_PUPD_PULLUP:	使用上拉电阻	
// 	@arg		GPIO_PUPD_PULLDOWN:	使用下拉电阻
// @param		otype:				引脚输出模式，只能选择以下参数：
// 	@arg		GPIO_OTYPE_PP:		推挽输出模式
// 	@arg		GPIO_OTYPE_OD:		开漏输出模式
// @param		speed:				引脚输出最大速度
// 	@arg		GPIO_OSPEED_2MHZ:	最大速度为10MHZ
// 	@arg		GPIO_OSPEED_25MHZ:	最大速度为2MHZ
// 	@arg		GPIO_OSPEED_50MHZ:	最大速度为50MHZ
// 	@arg		GPIO_OSPEED_200MHZ:	最大速度为200MHZ
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @return		void
// Sample usage:			gpio_output_init(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OSPEED_50MHZ, GPIO_OTYPE_PP, GPIO_PIN_0);
//                            //PA0初始化为推挽输出模式，最大输出速度为50MHZ，不使用上拉或下拉电阻
//-------------------------------------------------------------------------------------------------------------------
void gpio_output_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint8_t otype, uint32_t speed, uint32_t pin)
{
	gpio_mode_set(gpio_periph, mode, pull_up_down, pin);
	gpio_output_options_set(gpio_periph, otype, speed, pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO输入初始化
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G,H,I)
// @param		mode				GPIO的模式，只能选择以下参数：
// 	@arg		GPIO_MODE_INPUT:	输入模式
// 	@arg		GPIO_MODE_OUTPUT:	输出模式
// 	@arg		GPIO_MODE_AF:		复用模式
// 	@arg		GPIO_MODE_ANALOG:	模拟模式
// @param		pull_up_down		引脚使用上拉电阻还是下拉电阻，只能选择以下参数：
// 	@arg		GPIO_PUPD_NONE:		浮空模式，不使用电阻
// 	@arg		GPIO_PUPD_PULLUP:	使用上拉电阻	
// 	@arg		GPIO_PUPD_PULLDOWN:	使用下拉电阻
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @return		void
// Sample usage:			gpio_input_init(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
//                            //PA0初始化为输入模式，使用上拉电阻
//-------------------------------------------------------------------------------------------------------------------
void gpio_input_init(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin)
{
	gpio_mode_set(gpio_periph, mode, pull_up_down, pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO复用功能设置（来自官方底层）
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		alt_func_num:		选择的引脚复用功能，只能选择以下参数：
// 	@arg		GPIO_AF_0: 			SYSTEM
// 	@arg		GPIO_AF_1: 			TIMER0, TIMER1
// 	@arg		GPIO_AF_2: 			TIMER2, TIMER3, TIMER4
// 	@arg		GPIO_AF_3: 			TIMER7, TIMER8, TIMER9, TIMER10
// 	@arg		GPIO_AF_4: 			I2C0, I2C1, I2C2
// 	@arg		GPIO_AF_5: 			SPI0, SPI1, SPI2, SPI3, SPI4, SPI5
// 	@arg		GPIO_AF_6: 			SPI1, SPI2, SAI0 
// 	@arg		GPIO_AF_7: 			USART0, USART1, USART2
// 	@arg		GPIO_AF_8: 			UART3, UART4, USART5, UART6, UART7
// 	@arg		GPIO_AF_9: 			CAN0, CAN1, TLI, TIMER11, TIMER12, TIMER13
// 	@arg		GPIO_AF_10:			USB_FS, USB_HS
// 	@arg		GPIO_AF_11:			ENET
// 	@arg		GPIO_AF_12:			EXMC, SDIO, USB_HS
// 	@arg		GPIO_AF_13: 		DCI
// 	@arg		GPIO_AF_14: 		TLI
// 	@arg		GPIO_AF_15: 		EVENTOUT
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15)
// @return		void
// Sample usage:			gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_0);//PA0配置为复用功能0
//-------------------------------------------------------------------------------------------------------------------

//gpio_af_set(gpio_periph, alt_func_num, pin);


//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO输出设置
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @param		dat					0：低电平 1：高电平
// @return		void
// Sample usage:			gpio_set(GPIOA, GPIO_PIN_0, 1);//PA0输出高电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_output(uint32_t gpio_periph, uint32_t pin, uint8_t dat)
{
    if(RESET != dat){
        GPIO_BOP(gpio_periph) = (uint32_t)pin;
    }else{
        GPIO_BC(gpio_periph) = (uint32_t)pin;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO输出切换
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @return		void
// Sample usage:			gpio_toggle(GPIOA, GPIO_PIN_0, 1);//PA0输出切换（0变为1，1变为0）
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle(uint32_t gpio_periph, uint32_t pin)
{
    GPIO_TG(gpio_periph) = pin;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO获取输入值
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @return		uint8				0：低电平 1：高电平
// Sample usage:			uint8 status = gpio_get_input(GPIOA, GPIO_PIN_0);//获取PA0引脚输入电平
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get_input(uint32_t gpio_periph,uint32_t pin)
{
    if(0 != (GPIO_ISTAT(gpio_periph)&(pin))){
        return 1; 
    }else{
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO获取输出值
// @param		gpio_periph			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin					选择的引脚：GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
// @return		uint8				0：低电平 1：高电平
// Sample usage:			uint8 status = gpio_get_output(GPIOA, GPIO_PIN_0);//获取PA0引脚输出电平
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get_output(uint32_t gpio_periph,uint32_t pin)
{
    if(0 !=(GPIO_OCTL(gpio_periph)&(pin))){
        return 1;
    }else{
        return 0;
    }
}
