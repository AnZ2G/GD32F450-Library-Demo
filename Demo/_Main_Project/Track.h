/*********************************************************************************************************************
* @file				Track.h
* @author			胡籍
* @version			1.0
* @Target core		GD32F450ZIT6
* @board			LearnBoard_V1.3
* @date				2021-9-9
********************************************************************************************************************/

#include "common.h"

#ifndef TRACK_H
#define TRACK_H

/*** 宏定义 ****/ 

#define Black_Point 0
#define White_Point 255
#define RowMax	    60	  //行数
#define ColumnMax	94	  //列数
/*** 表达式定义 ****/ 
#define ABS(x) (((x) > 0) ? (x) : (-(x)))


/*** 外部函数变量申明 ****/  
extern char LastLine;
void SetInitVal(void);
void MiddleLineSearch1(void);
void MiddleLineSearch2(void);
void Middle_Print(void);
void Line_Print(void);
void Line_Repair(void);
void FireWater_Send(void);
uint8_t road_error(void);
uint16_t find_road_control(uint8_t error, uint8_t middle);


#include "gd32f4xx.h"
     


#endif
