/*!
    \file    readme.txt
    \brief   DCI摄像头测试
    
    \version 2021-4-7, V1.0.0, firmware for GD32F4xx
    \version 2021-4-10, V1.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何配置总钻风摄像头，
并通过F450的DCI资源获取图像，用有线串口发送至电脑。

-在GD32F450学习板上，PA6连接摄像头PCLK引脚，PA10
连接摄像头TXD引脚，PA9连接摄像头RXD引脚。PA4连接
摄像头NREF引脚，PB7连接摄像头VSY引脚，PC6、PC7、
PC8、PC9、PC11、PB6、PB8、PB9分别连接摄像头D0至
D7引脚。PC10连接有线串口TXD引脚，PC11连接有线串口
RXD引脚。

-打开电源后，串口向电脑发送测试程序。当收到“1、“2”、
“3”信息时，说明摄像头配置完成。随后持续向电脑发送
摄像头采集图像。可以使用山外多功能调试助手的摄像头图
像分析功能查看摄像头采集图像。