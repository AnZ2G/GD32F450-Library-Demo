/*!
    \file    readme.txt
    \brief   按键与拨码开关测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-8, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

* 例程说明：

-此例程基于GD32F450学习板, 展示如何配置按键、拨码开
关和LED的GPIO，并通过检测按键GPIO输入信号控制LED亮灭。

-在GD32F450学习板上，PD2连接LED1，PD0连接LED2，PD1
连接LED3。

-PD8、PD9、PD10、PD11、PD12、PD13连接按键。
在按下按键D8、D11时LED1会改变亮灭状态；
在按下按键D9、D10时LED2会改变亮灭状态；
在按下按键D12、D13时LED3会改变亮灭状态；
按住按键时，LED会闪烁。

-PG2、PG3、PG4、PG5连接拨码开关，在拨动拨码开关后，
LED闪烁的周期会改变。打开的拨码开关越多，LED闪烁周期
越长。

