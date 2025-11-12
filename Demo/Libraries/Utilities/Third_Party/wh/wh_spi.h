/********************************************************************************************************************
* @file				wh_spi.h
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#ifndef WH_SPI_H
#define WH_SPI_H

#include "gd32f4xx.h"


    /* 函数定义 */
void 		spi_NSS_init		(uint32_t NSS_gpio, uint32_t NSS_pin, rcu_periph_enum clk);
void 		spi_CLK_init		(uint32_t CLK_gpio, uint32_t CLK_pin, rcu_periph_enum clk);
void 		spi_MISO_init		(uint32_t MISO_gpio, uint32_t MISO_pin, rcu_periph_enum clk);
void 		spi_MOSI_init		(uint32_t MOSI_gpio, uint32_t MOSI_pin, rcu_periph_enum clk);
void 		spi_config			(uint32_t spi_periph, rcu_periph_enum rcu_periph);

#endif
