/*!
    \file    readme.txt
    \brief   舵机A3测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-8, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何应用中间层库pwm配置
函数输出pwm信号，对舵机角度进行控制。

-在GD32F450学习板上，PA3配置为复用功能2，作为TIMER4通
道3。通过中间层库函数对预分频值、自动重装载值、输出比较值
进行设置。PA3引脚输出pwm信号。根据pwm占空比（输出
比较值）对舵机转动角度进行控制。

-接通电源后，舵机来回往复转动。

