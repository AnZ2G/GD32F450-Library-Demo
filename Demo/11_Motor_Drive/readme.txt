/*!
    \file    readme.txt
    \brief   电机驱动测试
    
    \version 2021-3-24, V1.0.0, firmware for GD32F4xx
    \version 2021-4-2, V2.0.0, firmware for GD32F4xx
    \version 2021-4-8, V2.1.0, firmware for GD32F4xx
    \version 2021-7-8, V2.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2021, HeFei WeiHuang
*/

-此例程基于GD32F450学习板, 展示如何应用中间层库pwm配置
函数输出pwm信号，控制电机转动。

-在GD32F450学习板上，PA9，PA10配置为复用功能1，作为TIMER0通
道1,2。通过中间层库函数对预分频值、自动重装载值、输出比较值
进行设置。PA9引脚输出pwm信号，根据pwm占空比（输出比较值）对
电机转速进行控制。

-接通学习板电源和电机驱动板电源后，电机正转1S，停转1S，反转1S，停转1S。


