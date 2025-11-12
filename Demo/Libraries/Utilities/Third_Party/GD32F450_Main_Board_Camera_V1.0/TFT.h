/*********************************************************************************************************************
* @file				TFT.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_Camera_V1.0
* @date				2021-8-26
********************************************************************************************************************/

#ifndef TFT_H
#define TFT_H

#include "gd32f4xx.h"
     

/* 板载LED定义 */
#define TFT_RES_GPIO_PIN				GPIO_PIN_15
#define TFT_RES_GPIO_PORT				GPIOD
#define TFT_RES_GPIO_CLK				RCU_GPIOD
  
#define TFT_DC_GPIO_PIN					GPIO_PIN_14
#define TFT_DC_GPIO_PORT				GPIOB
#define TFT_DC_GPIO_CLK					RCU_GPIOB

#define TFT_MISO_GPIO_PIN				GPIO_PIN_15
#define TFT_MISO_GPIO_PORT				GPIOB
#define TFT_MISO_GPIO_CLK				RCU_GPIOB

#define TFT_SCK_GPIO_PIN				GPIO_PIN_13
#define TFT_SCK_GPIO_PORT				GPIOB
#define TFT_SCK_GPIO_CLK				RCU_GPIOB

#define TFT_CS_GPIO_PIN					GPIO_PIN_12
#define TFT_CS_GPIO_PORT				GPIOB
#define TFT_CS_GPIO_CLK					RCU_GPIOB

#define SPI_PERIPH						SPI1
#define SPI_CLK							RCU_SPI1


#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162
#define TFT18H        132

#define	RED		0xf800
#define	GREEN	0x07e0
#define	BLUE	0x001f
#define	PURPLE	0xf81f
#define	YELLOW	0xffe0
#define	CYAN	0x07ff 		//蓝绿色
#define	ORANGE	0xfc08
#define	BLACK	0x0000
#define	WHITE	0xffff

/* 函数定义 */

/* 初始化LED */
void TFT_SPI_Init(void);
/* 点亮LED */
void TFTSPI_Write_Cmd(uint8_t cmd);
/* 熄灭LED */
void TFTSPI_Write_Byte(uint8_t byte);
/* 切换LED状态 */
void TFTSPI_Write_Word(uint16_t dat);

void TFT_Init(uint8_t type);

void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);

void TFTSPI_Addr_Rst(void);

void TFTSPI_Fill_Area(uint16_t color);

void TFTSPI_CLS(uint16_t color);

void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat);

#endif
