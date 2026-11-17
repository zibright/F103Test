//
// Created by ZBright on 2026/11/11.
//

#ifndef HW_OLED_H
#define HW_OLED_H

#include <stdint.h>
#include "bsp_i2c.h"

#define OLED_ADDR 0x78
#define OLED_WIDTH  128
#define OLED_HEIGHT 64


void OLED_Init(I2C_Ops_t* bus);
void OLED_Clear(void);
void OLED_Refresh(void); // 将内部显存刷新到屏幕

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);

void OLED_ShowChinese(uint8_t x, uint8_t y, char* chn);

#endif //HW_OLED_H
