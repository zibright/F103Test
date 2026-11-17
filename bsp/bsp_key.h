#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "stm32f10x.h"
#include <stdint.h>

#define KEY_COUNT				4

#define KEY_1					0
#define KEY_2					1
#define KEY_3					2
#define KEY_4					3

/* ======== 引脚配置 ========= */
// 假设按键为上拉输入，按下为低电平(0)

// KEY1 -> PB0
#define KEY1_RCC    RCC_APB2Periph_GPIOB
#define KEY1_PORT   GPIOB
#define KEY1_PIN    GPIO_Pin_0

// KEY2 -> PA6
#define KEY2_RCC    RCC_APB2Periph_GPIOA
#define KEY2_PORT   GPIOA
#define KEY2_PIN    GPIO_Pin_6

// KEY3 -> PA4
#define KEY3_RCC    RCC_APB2Periph_GPIOA
#define KEY3_PORT   GPIOA
#define KEY3_PIN    GPIO_Pin_4

// KEY4 -> PA2
#define KEY4_RCC    RCC_APB2Periph_GPIOA
#define KEY4_PORT   GPIOA
#define KEY4_PIN    GPIO_Pin_2

void    BSP_Key_Init(void);
uint8_t BSP_Key_Read(uint8_t n); // 返回 0 表示按下，1 表示松开

#endif // BSP_KEY_H