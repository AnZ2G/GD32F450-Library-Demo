/*********************************************************************************************************************
* @file				main.c
* @brief			Ö÷³ÌÐò
* @author			HeFei WeiHuang
* @version			1.0
* @Target core		GD32F450ZIT6
* @date				2021-8-31
********************************************************************************************************************/

#include "gd32f4xx.h"
#include "common.h"
#include "headfile.h"
#include "systick.h"
#include <stdio.h>


int main(void)
{
    start_up();

    while(1)
    {

    }
}

void start_up(void)
{
    systick_config();
}



