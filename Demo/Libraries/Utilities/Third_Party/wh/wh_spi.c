/********************************************************************************************************************
* @file				wh_spi.c
* @author			Andreas WH
* @version			2.0
* @Target core		gd32f450
* @date				2021-4-6
********************************************************************************************************************/

#include "wh_spi.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置NSS引脚
// @param		NSS_gpio			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		NSS_pin				选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk					gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			spi_NSS_init(GPIOE, GPIO_PIN_11, RCU_GPIOE);//PE11作为NSS引脚
//-------------------------------------------------------------------------------------------------------------------
void spi_NSS_init(uint32_t NSS_gpio, uint32_t NSS_pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
    gpio_mode_set(NSS_gpio, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, NSS_pin);
    gpio_output_options_set(NSS_gpio, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, NSS_pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置SCK引脚
// @param		SCK_gpio			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		SCK_pin				选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk					gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			spi_SCK_init(GPIOE, GPIO_PIN_12, RCU_GPIOE);//PE12作为SCK引脚
//-------------------------------------------------------------------------------------------------------------------
void spi_CLK_init(uint32_t CLK_gpio, uint32_t CLK_pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
    gpio_af_set(CLK_gpio, GPIO_AF_5, CLK_pin);
    gpio_mode_set(CLK_gpio, GPIO_MODE_AF, GPIO_PUPD_PULLUP, CLK_pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置MISO引脚
// @param		MISO_gpio			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		MISO_pin			选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk					gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			spi_MISO_init(GPIOE, GPIO_PIN_13, RCU_GPIOE);//PE13作为MISO引脚
//-------------------------------------------------------------------------------------------------------------------
void spi_MISO_init(uint32_t MISO_gpio, uint32_t MISO_pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
    gpio_af_set(MISO_gpio, GPIO_AF_5, MISO_pin);
    gpio_mode_set(MISO_gpio, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MISO_pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置MOSI引脚
// @param		MOSI_gpio			选择的端口：GPIOx(x = A,B,C,D,E,F,G)
// @param		MOSI_pin			选择的引脚：GPIO_PIN_x(x=0..15)
// @param		clk					gpio的时钟端口：RCU_GPIOx(x = A,B,C,D,E,F,G)
// @return		void
// Sample usage:			spi_MOSI_init(GPIOE, GPIO_PIN_14, RCU_GPIOE);//PE14作为MOSI引脚
//-------------------------------------------------------------------------------------------------------------------
void spi_MOSI_init(uint32_t MOSI_gpio, uint32_t MOSI_pin, rcu_periph_enum clk)
{
	rcu_periph_clock_enable(clk);
    gpio_af_set(MOSI_gpio, GPIO_AF_5, MOSI_pin);
    gpio_mode_set(MOSI_gpio, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MOSI_pin);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		配置SPI
// @param		uint32_t spi_periph		需要配置的SPI外设号
// @return		void
// Sample usage:			spi_config(SPI1);//配置SPI1
//-------------------------------------------------------------------------------------------------------------------
void spi_config(uint32_t spi_periph, rcu_periph_enum rcu_periph)
{

    rcu_periph_clock_enable(rcu_periph);
    spi_i2s_deinit(spi_periph);
    /* 定义SPI结构体 */
    spi_parameter_struct spi_init_struct;

    /* 配置SPI结构体并将其配 置到所用SPI */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;

    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(spi_periph, &spi_init_struct);
	

    /* 使能SPI */
    spi_enable(spi_periph);
}

