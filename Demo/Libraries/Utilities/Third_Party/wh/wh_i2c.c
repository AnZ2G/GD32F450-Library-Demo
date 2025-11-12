/********************************************************************************************************************
* @file				wh_spi.c
* @author			Andreas WH
* @version			1.0
* @Target core		gd32f450
* @date				2021-8-4
********************************************************************************************************************/

#include "wh_i2c.h"
#include "wh_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置I2C(I2C1)
// @param		i2c_speed			i2c速度
// @param		i2c_addr			i2c地址
// @return		void
// Sample usage:			i2c_config(400000, 0x00);//配置I2C1速度为400000,地址为0x00
//-------------------------------------------------------------------------------------------------------------------
void i2c_config(uint32_t i2c_speed, uint32_t i2c_addr)
{
    /* 使能GPIOF时钟 */
    rcu_periph_clock_enable(RCU_GPIOF);
	/* GPIO 初始化 */
    gpio_output_init(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_output_init(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* GPIO复用功能配置 */
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_1);
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_0);

    /* 使能I2C时钟 */
    rcu_periph_clock_enable(RCU_I2C1);
    /* 配置I2C时钟 */
    i2c_clock_config(I2C1, i2c_speed, I2C_DTCY_2);
    /* 配置I2C地址 */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, i2c_addr);
    /* 使能I2C */
    i2c_enable(I2C1);
    /* I2C允许应答 */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		I2C发送字节
// @param		slave_address	从机地址
// @param		dat					发送的字节
// @return		void
// Sample usage:			i2c_send_byte(0x68, 0x21);
                          //I2C1开始通信,发送从机地址0x68,发送数据0x21
//-------------------------------------------------------------------------------------------------------------------
void i2c_send_byte(uint8_t slave_addr, uint8_t dat)
{
    /* 等待I2C总线空闲 */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    /* 发送起始信号 */
    i2c_start_on_bus(I2C1);
    /* 等待起始信号发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    /* 发送从机地址 */
    i2c_master_addressing(I2C1, slave_addr, I2C_TRANSMITTER);    //发送ICM20602地址
    /* 等待地址发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)){;}	
    /* 地址发送标志位清除 */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND); 
    /* 等待从机应答 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));

    /* 发送字节 */
    i2c_data_transmit(I2C1, dat);
    /* 等待发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

    /* I2C放开总线 */
    i2c_stop_on_bus(I2C1);
    /* 等待总线释放 */
    while(I2C_CTL0(I2C1)&0x0200);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		I2C向寄存器发送字节
// @param		slave_address		从机地址
// @param		reg_address			寄存器地址
// @param		dat					发送的字节
// @return		void
// Sample usage:			i2c_send_reg_byte(0x68, 0x34, 0x21);
                          //I2C1开始通信,发送从机地址0x68,发送寄存器地址0x34,发送数据0x21
//-------------------------------------------------------------------------------------------------------------------
void i2c_send_reg_byte(uint8_t slave_addr, uint8_t reg_addr, uint8_t dat)
{
    /* 等待I2C总线空闲 */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    /* 发送起始信号 */
    i2c_start_on_bus(I2C1);
    /* 等待起始信号发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    /* 发送从机地址 */
    i2c_master_addressing(I2C1, slave_addr, I2C_TRANSMITTER);    //发送ICM20602地址
    /* 等待地址发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND)){;}	
    /* 地址发送标志位清除 */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND); 
    /* 等待从机应答 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));

    /* 发送寄存器地址 */
    i2c_data_transmit(I2C1, reg_addr);
    /* 等待发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

    /* 发送字节 */
    i2c_data_transmit(I2C1, dat);
    /* 等待发送完毕 */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

    /* I2C放开总线 */
    i2c_stop_on_bus(I2C1);
    /* 等待总线释放 */
    while(I2C_CTL0(I2C1)&0x0200);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		I2C读取数组
// @param		p_buffer			读取的字节存放的数组地址
// @param		slave_address		读取的从机的地址
// @param		reg_address			读取的寄存器的地址
// @param		number_of_byte		读取的字节数量
// @return		void			
// Sample usage:				uint8_t data[3]; i2c_read_buffer(0x68, 0x75, &data, 3);
                                // I2C1读取字节，存放在data
//-------------------------------------------------------------------------------------------------------------------
void i2c_read_buffer(uint8_t slave_address, uint8_t reg_address, uint8_t* p_buffer, uint16_t number_of_byte)
{  
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));

    if(2 == number_of_byte){
        i2c_ackpos_config(I2C1,I2C_ACKPOS_NEXT);
    }
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, slave_address, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C1 , I2C_FLAG_TBE));

    /* enable I2C0*/
    i2c_enable(I2C1);
    
    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, reg_address);  
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, slave_address, I2C_RECEIVER);

    if(number_of_byte < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C1,I2C_ACK_DISABLE);
    }
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1,I2C_FLAG_ADDSEND);
    
    if(1 == number_of_byte){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C1);
    }
    
    /* while there is data to be read */
    while(number_of_byte){
        if(3 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C1,I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C1);
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C1, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *p_buffer = i2c_data_receive(I2C1);
            
            /* point to the next location where the byte read will be saved */
            p_buffer++; 
            
            /* decrement the read bytes counter */
            number_of_byte--;
        } 
    }
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C1,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C1,I2C_ACKPOS_CURRENT);
}
