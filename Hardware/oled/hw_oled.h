//
// Created by ZBright on 2026/11/11.
//

#ifndef HW_OLED_H
#define HW_OLED_H

#include <stdint.h>
#include "bsp_i2c.h"


#define OLED_WIDTH  128
#define OLED_HEIGHT 64

// 驱动芯片宏开关：默认 SSD1306
// 需要 SH1106 时，在编译器的预处理宏（如 Keil 的 Define、CMake 的 -D）中添加 SH_1106
// #define SH_1106

#ifndef SH_1106
#define SSD_1306
#define OLED_ADDR   0x78
#define OLED_IS_SH1106   0
#else
#define OLED_ADDR   0x7A
#define OLED_IS_SH1106   1
#endif


void OLED_Init(I2C_Ops_t* bus); // 初始化（芯片类型由上方宏自动决定）
void OLED_Clear(void);
void OLED_Refresh(void); // 将内部显存刷新到屏幕

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);

void OLED_ShowChinese(uint8_t x, uint8_t y, char* chn);

#endif //HW_OLED_H