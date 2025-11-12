/*********************************************************************************************************************
* @file				Image.h
* @author			ºú¼®
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-9-9
********************************************************************************************************************/

#ifndef _IMAGE_H
#define _IMAGE_H
#include "gd32f4xx.h"
#include "SEEKFREE_MT9V03X.h"
#include "Track.h"
/*** ºê¶¨Òå ****/ 
#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(MAX)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
     
extern uint8_t IMG_zip[60][94];
uint8_t my_adapt_threshold_otso(uint8_t tmImage[RowMax][ColumnMax], uint16_t col, uint16_t row);
uint8_t ostu1(void);
void ImRez(void);
void ImRez_ori(void);
#endif
