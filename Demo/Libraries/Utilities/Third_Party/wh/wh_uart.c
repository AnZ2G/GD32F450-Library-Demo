/*********************************************************************************************************************
* @file				wh_uart.c
* @author			Andreas WH
* @version			3.0
* @Target core		GD32F450ZIT6
* @date				2021-8-15
********************************************************************************************************************/

#include "wh_uart.h"
#include "wh_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		Tx引脚初始化
// @param		gpio_periph		选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin				选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk				gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			uart_tx_init(GPIOA, GPIO_PIN_9, RCU_GPIOA);
                            //PA9作为Tx引脚初始化
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_init(uint32_t gpio_periph, uint32_t pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
	gpio_af_set(gpio_periph, GPIO_AF_7, pin);
	gpio_output_init(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Rx引脚初始化
// @param		gpio_periph		选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		pin				选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk				gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			uart_rx_init(GPIOA, GPIO_PIN_10, RCU_GPIOA);
                            //PA10作为Rx引脚初始化
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_init(uint32_t gpio_periph, uint32_t pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
	gpio_af_set(gpio_periph, GPIO_AF_7, pin);
	gpio_output_init(gpio_periph, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口初始化(已设置TX和RX引脚)
// @param		uart_clk		串口时钟端口：RCU_USARTx(x = 0,1,2),RCU_UARTx(x = 3,4)
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		baud			波特率
// @return		void			
// Sample usage:				uart_init(RCU_USART0, USART0, 115200U)	
                                // 初始化串口USART0 波特率115200 发送引脚和接收引脚在之前已经设置
//-------------------------------------------------------------------------------------------------------------------
void uart_init(rcu_periph_enum uart_clk, uint32_t uart_periph, uint32_t baud)
{
    rcu_periph_clock_enable(uart_clk);													//配置串口时钟
    usart_deinit(uart_periph);                                                          //串口初始化
    usart_baudrate_set(uart_periph, baud);                                              //配置波特率
    usart_receive_config(uart_periph, USART_RECEIVE_ENABLE);                            //使能接收
    usart_transmit_config(uart_periph, USART_TRANSMIT_ENABLE);                          //使能发送
    usart_enable(uart_periph);                                                          //使能串口
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口字节输出
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		dat				需要发送的字节
// @return		void        
// Sample usage:				uart_putchar(USART0,0xA5);								// 串口USART0发送0xA5
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(uint32_t uart_periph, uint8_t data)
{
    usart_data_transmit(uart_periph, data);                                             //写入数据
	while(!(usart_flag_get(uart_periph,USART_FLAG_TBE)));									// 等待发送完成
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送数组
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		*buff			要发送的数组地址
// @param		len				发送长度
// @return		void
// Sample usage:				uart_putbuff(USART0,&a[0],5);
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(uint32_t uart_periph, uint8_t *buff, uint32_t len)
{
	while(len)																			// 循环到发送完
	{
        usart_data_transmit(uart_periph, *buff++);                                      //写入数据
		while(!(usart_flag_get(uart_periph,USART_FLAG_TBE)));								// 等待发送完成
		len--;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送字符串
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		*str			要发送的字符串地址
// @return		void
// Sample usage:				uart_putstr(USART0,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(uint32_t uart_periph, const uint8_t *str)
{
	while(*str)																			// 一直循环到结尾
	{
		usart_data_transmit(uart_periph, *str++);										// 发送数据
		while(!(usart_flag_get(uart_periph,USART_FLAG_TBE)));
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取串口接收的数据（whlie等待）
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		*dat			接收数据的地址
// @return		void        
// Sample usage:				uint32_t dat; uart_getchar(USART0,&dat);				// 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar(uint32_t uart_periph, uint8_t *dat)
{
	while(!(usart_flag_get(uart_periph,USART_FLAG_RBNE)));									// 等待读取到一个数据
	*dat = usart_data_receive(uart_periph);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取串口接收的数据（查询接收）
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		*dat			接收数据的地址
// @return		uint8			1：接收成功   0：未接收到数据
// Sample usage:				uint8_t dat; uart_query(USART0,&dat);					// 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8_t uart_query(uint32_t uart_periph, uint8_t *dat)
{
	if(usart_flag_get(uart_periph,USART_FLAG_RBNE))											// 读取到一个数据
	{
		*dat = usart_data_receive(uart_periph);										// 存储一个数据
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口中断设置
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		int_flag		串口中断名称
//								只能选择以下参数：
//  \arg        USART_INT_PERR:		校验错误中断
//  \arg        USART_INT_TBE:		发送缓冲区空中断
//  \arg        USART_INT_TC:		发送完成中断
//  \arg        USART_INT_RBNE:		读数据缓冲区非空中断和过载错误中断
//  \arg        USART_INT_IDLE:		IDLE线检测中断
//  \arg        USART_INT_LBD:		LIN断开信号检测中断
//  \arg        USART_INT_ERR:		错误中断
//  \arg        USART_INT_CTS:		CTS中断
//  \arg        USART_INT_RT:		接收超时事件中断
//  \arg        USART_INT_EB:		块结束事件中断
// @param		status			1：打开中断   0：关闭中断
// @param		uart_irq		串口中断号：USARTx_IRQn(x = 0,1,2),UARTx_IRQn(x = 3,4)
// @return		void        
// Sample usage:				usat_interrupt_init(USART0, USART_INT_TC, 1, USART0_IRQn);	// 打开串口1发送完成中断,并设置优先级
//-------------------------------------------------------------------------------------------------------------------

void uart_interrupt_init(uint32_t uart_periph, usart_interrupt_enum int_flag, uint32_t status, uint32_t uart_irq)
{
	if(status)
		usart_interrupt_enable(uart_periph, int_flag);			                        // 使能串口中断
	else
		usart_interrupt_disable(uart_periph, int_flag);							   		// 关闭串口中断

	nvic_irq_enable(uart_irq, 0, 0);													// 设置抢占优先级和响应优先级
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		获取串口中断标志位值
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		int_flag		串口中断名称
//								只能选择以下参数：
//  \arg        USART_INT_PERR:		校验错误中断
//  \arg        USART_INT_TBE:		发送缓冲区空中断
//  \arg        USART_INT_TC:		发送完成中断
//  \arg        USART_INT_RBNE:		读数据缓冲区非空中断和过载错误中断
//  \arg        USART_INT_IDLE:		IDLE线检测中断
//  \arg        USART_INT_LBD:		LIN断开信号检测中断
//  \arg        USART_INT_ERR:		错误中断
//  \arg        USART_INT_CTS:		CTS中断
//  \arg        USART_INT_RT:		接收超时事件中断
//  \arg        USART_INT_EB:		块结束事件中断
// @return		uint8			1：标志位为1   0：标志位为0        
// Sample usage:				uint_8 status; status = uart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE);//获取UART0读数据缓冲区非空中断标志位值
//-------------------------------------------------------------------------------------------------------------------
uint8_t uart_interrupt_flag_get(uint32_t uart_periph, usart_interrupt_enum int_flag)
{
    uint32_t intenable = 0U, flagstatus = 0U;
    /* get the interrupt enable bit status */
    intenable = (USART_REG_VAL(uart_periph, int_flag) & BIT(USART_BIT_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (USART_REG_VAL2(uart_periph, int_flag) & BIT(USART_BIT_POS2(int_flag)));

    if(flagstatus && intenable){
        return 1;
    }else{
        return 0; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		清除串口中断标志位值
// @param		uart_periph		串口模块号：USARTx(x = 0,1,2),UARTx(x = 3,4)
// @param		int_flag		串口中断名称
//								只能选择以下参数：
//  \arg        USART_INT_PERR:		校验错误中断
//  \arg        USART_INT_TBE:		发送缓冲区空中断
//  \arg        USART_INT_TC:		发送完成中断
//  \arg        USART_INT_RBNE:		读数据缓冲区非空中断和过载错误中断
//  \arg        USART_INT_IDLE:		IDLE线检测中断
//  \arg        USART_INT_LBD:		LIN断开信号检测中断
//  \arg        USART_INT_ERR:		错误中断
//  \arg        USART_INT_CTS:		CTS中断
//  \arg        USART_INT_RT:		接收超时事件中断
//  \arg        USART_INT_EB:		块结束事件中断
// @return		void        
// Sample usage:				uart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);//清除UART0读数据缓冲区非空中断标志位值
//-------------------------------------------------------------------------------------------------------------------
void uart_interrupt_flag_clear(uint32_t uart_periph, usart_interrupt_flag_enum int_flag)
{
    USART_REG_VAL2(uart_periph, int_flag) &= ~BIT(USART_BIT_POS2(int_flag));
}
