/********************************************************************************************************************
* @file				wh_spi.h
* @author			Andreas WH
* @version			1.0
* @Target core		gd32f450
* @date				2021-8-4
********************************************************************************************************************/

#ifndef WH_I2C_H
#define WH_I2C_H

#include "gd32f4xx.h"

    /* 函数定义 */
void i2c_config(uint32_t i2c_speed, uint32_t i2c_addr);
void i2c_send_byte(uint8_t slave_addr, uint8_t dat);
void i2c_send_reg_byte(uint8_t slave_addr, uint8_t reg_addr, uint8_t dat);
void i2c_read_buffer(uint8_t slave_address, uint8_t reg_address, uint8_t* p_buffer, uint16_t number_of_byte);

#endif
