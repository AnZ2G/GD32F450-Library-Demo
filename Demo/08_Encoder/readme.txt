/*!
    \file    readme.txt
    \brief   编码器测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-8, V2.1.0, firmware for GD32F4xx
    \version 2021-7-7, V2.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何配置TIMER2的正交解码
器和计数功能，从而对编码器输出的脉冲电平进行解码并计数，
通过有线串口将数据发送至电脑。

-在GD32F450学习板上，PB10连接TXD引脚，PB11连接RXD引脚，
PB0、PB5连接编码器输出脚。配置PB0、PB5为复用功能2，作为
TIMER2通道，用于接收编码器输出。使用中间层库函数配置定时器
结构体，并通过底层库函数配置TIMER2的预分频器、重装载、正交
解码器等功能。启动编码器，编码器根据转动圈数输出脉冲信号，
经过解码并计数，通过有线串口发送至电脑。

-打开电源，持续向电脑发送编码器转动情况，间隔1s。

