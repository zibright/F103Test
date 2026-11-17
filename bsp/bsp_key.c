#include "bsp_key.h"

void BSP_Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启所需端口的时钟 (PA 和 PB)
    RCC_APB2PeriphClockCmd(KEY1_RCC | KEY2_RCC | KEY3_RCC | KEY4_RCC, ENABLE);

    // 统一配置为上拉输入 (默认高电平，按下为低电平)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 按键1 (PB0)
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);

    // 按键2 (PA6)
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY2_PORT, &GPIO_InitStructure);

    // 按键3 (PA4)
    GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY3_PORT, &GPIO_InitStructure);

    // 按键4 (PA2)
    GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
    GPIO_Init(KEY4_PORT, &GPIO_InitStructure);
}

uint8_t BSP_Key_Read(uint8_t n)
{
    switch (n) {
        case KEY_1: return GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN);
        case KEY_2: return GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN);
        case KEY_3: return GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN);
        case KEY_4: return GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN);
        default: return 1; // 默认返回未按下 (高电平)
    }
}