/*********************************************************************************************************************
* @file				gd32f450_board.h
* @author			Andreas WH
* @version			1.0
* @Target core		gd32f450
* @date				2021-3-20
********************************************************************************************************************/

#ifndef GD32F450_BOARD_H
#define GD32F450_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
/* 类型定义 */
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2
} led_typedef_enum;

/* 板载LED定义 */
#define LEDn                             3U

#define LED1_PIN                         GPIO_PIN_2
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCU_GPIOD
  
#define LED2_PIN                         GPIO_PIN_0
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCU_GPIOD
  
#define LED3_PIN                         GPIO_PIN_1
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCU_GPIOD

/* 函数定义 */
void 	led_init		(led_typedef_enum lednum);
void 	led_on			(led_typedef_enum lednum);
void 	led_off			(led_typedef_enum lednum);
void 	led_toggle		(led_typedef_enum lednum);

#ifdef __cplusplus
}
#endif

#endif /* GD32F450I_EVAL_H */
