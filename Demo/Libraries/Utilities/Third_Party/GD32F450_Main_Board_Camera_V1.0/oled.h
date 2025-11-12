/********************************************************************************************************************
* @file				oled.h
* @author			Andreas WH
* @version			1.0
* @Target core		gd32f450
* @date				2021-8-5
********************************************************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "systick.h"
#include "gd32f4xx.h"
#include "SEEKFREE_MT9V03X.h"
 
//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		0x00
#define XLevelH		0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	
						 
//-------------写命令和数据定义-------------------
#define OLED_CMD     0x00	//写命令
#define OLED_DATA    0x40	//写数据 
   						  
//-----------------OLED端口定义----------------
//#define OLED_CS   GPIO_PIN_11   //片选信号           PD11
#define OLED_DC   GPIO_PIN_12   //数据/命令控制信号   PD5
#define OLED_RST  GPIO_PIN_14   //复位信号           PD12
#define OLED_SPI  SPI1

extern uint8_t Img[MT9V03X_H][MT9V03X_W];

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Init(void);
void oled_set_addr(unsigned char page, unsigned char column);
void Disp_8x16(unsigned char page, unsigned char column, unsigned char reverse, unsigned char *dp);
void Disp_16x16(unsigned char page, unsigned char column, unsigned char reverse, unsigned char *Chinese);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t Char_Size);
void Clr_Screen(unsigned char page);
void OLED_ShowChar_1(uint8_t y,uint8_t x,uint8_t chr,uint8_t Char_Size);
void print_str8x16(uint8_t y,uint8_t x,uint8_t Char_Size,uint8_t *chr);
void print_char8x16(uint8_t y,uint8_t x,uint8_t Char_Size,uint8_t *chr);
void clr_Screen(unsigned char page,unsigned char start,unsigned char end);
void Bmp_display(uint8_t picture_buffer[64][128]);
void OLED_camera(uint8_t value);
void OLED_camera_zip(uint8_t value);
void OLED_camera_any(uint8_t buffer[60][94], uint8_t value);
void OLED_ShowSchool(uint8_t y, uint8_t pos);
void OLED_ShowSchool_1(uint8_t y, uint8_t pos);
void OLED_OpeningAnimation(void);
#endif
