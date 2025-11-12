/*!
    \file    readme.txt
    \brief   OLED（硬件SPI）测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-10, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何应用外部OLED库和SPI库
配置OLED，进行一定的显示操作

-在GD32F450学习板上，PB12连接CS引脚，PB14连接DC引脚，
PB15连接SD引脚，PB13连接CK引脚。使用外部OLED库和SPI库
对SPI进行配置，并通过SPI通讯函数控制OLED屏显示情况。

-将OLED屏接入学习板对应位置后，接通电源，OLED屏闪烁
一次，随后显示“合肥炜煌 Hello World！”字样。“！”闪烁
，间隔1s。
