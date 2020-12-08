#ifndef LEDS_APP_H
#define LEDS_APP_H

#include "gd32f30x.h"
/* 宏定义 */
#define LEDn                        3U
#define GREEN_PIN                   GPIO_PIN_1
#define GREEN_GPIO_PORT             GPIOA
#define GREEN_GPIO_CLK              RCU_GPIOA
#define BLUE_PIN                    GPIO_PIN_2
#define BLUE_GPIO_PORT              GPIOA
#define BLUE_GPIO_CLK               RCU_GPIOA
#define RED_PIN                     GPIO_PIN_6
#define RED_GPIO_PORT               GPIOB
#define RED_GPIO_CLK                RCU_GPIOB
/* 枚举结构体 */
typedef enum 
{
    GREEN = 0,
    BLUE = 1,
    RED = 2
} led_typedef_enum;
/* 初始化IO口函数 */
void led_init(led_typedef_enum lednum);
/* 翻转IO口电平函数 */
void led_toggle(led_typedef_enum lednum);

#endif
