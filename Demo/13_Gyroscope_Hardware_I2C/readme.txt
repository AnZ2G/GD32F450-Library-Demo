/*!
    \file    readme.txt
    \brief   陀螺仪（硬件SPI）测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-10, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板,  展示如何应用中间层库配置SPI
应用外部陀螺仪库配置陀螺仪，并将陀螺仪加速度和姿态信息通
过有线串口发送至电脑。

-在GD32F450学习板上，PC10连接TXD引脚，PC11连接RXD引脚，
串口使用USART2，波特率设置为115200。PE11配置为NSS引脚，
PE12配置为CLK引脚，PE13配置为MISO引脚，PE14配置为MOSI
引脚。通过外部陀螺仪库函数获取加速度和姿态信息，并通过
有线串口将信息发送至电脑。

-将陀螺仪接入学习板对于位置。打开电源后，通过有线串口
向电脑发送加速度和姿态数据，间隔1s。
