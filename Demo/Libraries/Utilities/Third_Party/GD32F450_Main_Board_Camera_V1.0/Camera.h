/*********************************************************************************************************************
* @file				Camera.h
* @author			Andreas WH
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			Main_Board_V1.0
* @date				2021-8-31
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "SEEKFREE_MT9V03X.h"


#ifndef _CAMERA_H
#define _CAMERA_H


void Camera_Init(void);
void Binarization(uint8_t* picture_buffer, uint8_t value);
void Camera_uart_display(void);
void Camera_uart_display_BI(void);
void Camera_oled_display(uint8_t value);
void Camera_tft_display(void);
uint8_t OSTU(void);
uint8_t ostu(void);

#endif
