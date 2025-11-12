/*********************************************************************************************************************
* @file				UART.c
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-8-26
********************************************************************************************************************/

#include "UART.h"
#include "headfile.h"


/* 类型定义 */
static uint32_t UART_GPIO_PORT[UARTn] = {Wired_UART_GPIO_PORT, Wireless_UART_GPIO_PORT};

static uint32_t UART_TXD_PIN[UARTn] = {Wired_UART_TXD_PIN, Wireless_UART_TXD_PIN};

static uint32_t UART_RXD_PIN[UARTn] = {Wired_UART_RXD_PIN, Wireless_UART_RXD_PIN};

static rcu_periph_enum UART_GPIO_CLK[UARTn] = {Wired_UART_GPIO_CLK, Wireless_UART_GPIO_CLK};

static rcu_periph_enum UART_CLK[UARTn] = {Wired_UART_CLK, Wireless_UART_CLK};

static uint32_t UART_PERIPH[UARTn] = {Wired_UART_PERIPH, Wireless_UART_PERIPH};

static uint8_t UART_nvic[UARTn] = {Wired_UART_nvic, Wireless_UART_nvic};

uart_typedef_enum uart_periph;

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口初始化
// @param		uartnum				选择的串口：Wired_UART,Wireless_UART
// @param		Baud				波特率
// @return		void
// Sample usage:			UART_Init(Wired_UART, 115200);//初始化有线串口，波特率为115200.
//-------------------------------------------------------------------------------------------------------------------
void UART_Init(uart_typedef_enum uartnum, uint32_t baud)
{
    /* 配置UART中断优先级 */
    nvic_irq_enable(UART_nvic[uartnum], 0, 0);

    /* 配置TXD引脚 */
    uart_tx_init(UART_GPIO_PORT[uartnum], UART_TXD_PIN[uartnum], UART_GPIO_CLK[uartnum]);

    /* 配置RXD引脚 */
    uart_rx_init(UART_GPIO_PORT[uartnum], UART_RXD_PIN[uartnum], UART_GPIO_CLK[uartnum]);

    /* 配置UART*/
    uart_init(UART_CLK[uartnum], UART_PERIPH[uartnum], baud);

    uart_periph = uartnum;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口字节输出
// @param		dat				需要发送的字节
// @return		void        
// Sample usage:				UART_Putchar(0xA5);	// 串口发送0xA5
//-------------------------------------------------------------------------------------------------------------------
void UART_Putchar(uint8_t data)
{
    usart_data_transmit(UART_PERIPH[uart_periph], data);                                            //写入数据
	while(!(usart_flag_get(UART_PERIPH[uart_periph],USART_FLAG_TBE)));								// 等待发送完成
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送数组
// @param		*buff			要发送的数组地址
// @param		len				发送长度
// @return		void
// Sample usage:				uint8_t a[5]; UART_Putbuff(a,4);// 串口发送数组a
//-------------------------------------------------------------------------------------------------------------------
void UART_Putbuff(uint8_t *buff, uint32_t len)
{
	while(len)																						// 循环到发送完
	{
        usart_data_transmit(UART_PERIPH[uart_periph], *buff++);                                     //写入数据
		while(!(usart_flag_get(UART_PERIPH[uart_periph],USART_FLAG_TBE)));							// 等待发送完成
		len--;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		串口发送字符串
// @param		*str			要发送的字符串地址
// @return		void
// Sample usage:				UART_Putstr("i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void UART_Putstr(const uint8_t *str)
{
	while(*str)																						// 一直循环到结尾
	{
		usart_data_transmit(UART_PERIPH[uart_periph], *str++);										// 发送数据
		while(!(usart_flag_get(UART_PERIPH[uart_periph],USART_FLAG_TBE)));
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取串口接收的数据（whlie等待）
// @param		*dat			接收数据的地址
// @return		void        
// Sample usage:				uint32_t dat; UART_Getchar(&dat); // 接收串口数据，存放在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void UART_Getchar(uint8_t *dat)
{
	while(!(usart_flag_get(UART_PERIPH[uart_periph],USART_FLAG_RBNE)));								// 等待读取到一个数据
	*dat = usart_data_receive(UART_PERIPH[uart_periph]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取串口接收的数据（查询接收）
// @param		*dat			接收数据的地址
// @return		uint8			1：接收成功   0：未接收到数据
// Sample usage:				uint8_t dat; UART_Query(&dat);	// 接收串口数据，存放在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8_t UART_Query(uint8_t *dat)
{
	if(usart_flag_get(UART_PERIPH[uart_periph],USART_FLAG_RBNE))									// 读取到一个数据
	{
		*dat = usart_data_receive(UART_PERIPH[uart_periph]);										// 存储一个数据
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		使能串口接收中断
// @return		void        
// Sample usage:				UART_Interrupt_Enable();	//使能串口接收中断 
//-------------------------------------------------------------------------------------------------------------------
void UART_Interrupt_Enable(void)
{
    usart_interrupt_enable(UART_PERIPH[uart_periph], USART_INT_RBNE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		失能串口接收中断
// @return		void        
// Sample usage:				UART_Interrupt_Enable();	//失能串口接收中断 
//-------------------------------------------------------------------------------------------------------------------
void UART_Interrupt_Disable(void)
{
    usart_interrupt_disable(UART_PERIPH[uart_periph], USART_INT_RBNE);
}

/* 重定向printf函数 */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(UART_PERIPH[uart_periph], (uint8_t)ch);
    while(RESET == usart_flag_get(UART_PERIPH[uart_periph], USART_FLAG_TBE));
    return ch;
}
