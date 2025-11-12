/*!
    \file    readme.txt
    \brief   无线通信测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何配置TXD引脚、RXD硬件和
USART，并通过无线串口模块向电脑发送信息

-在GD32F450学习板上，PD5连接TXD引脚，PD6连接RXD引脚，
串口使用USART1，波特率设置为115200。

-开启程序后通过串口持续发送消息“a usart transmit test example!”，
间隔1s。成功接收信息时，无线串口USB段模块LED闪烁一次。

