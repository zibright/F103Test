//
// Created by ZBright on 2026/11/11.
//

#include "hw_oled.h"
#include "oled_fonts.h"
#include <stddef.h>

// 显存：8页，每页128列
uint8_t OLED_GRAM[8][128] = {0};
static I2C_Ops_t* oled_bus = NULL;

// 写命令
static void OLED_WriteCmd(uint8_t cmd) {
    oled_bus->write(OLED_ADDR, 0x00, &cmd, 1);
}

void OLED_Init(I2C_Ops_t* bus) {
    oled_bus = bus; // 绑定注入的总线
    oled_bus->init(); // 初始化总线

    // SSD1306 初始化序列 (常用精简版)
    OLED_WriteCmd(0xAE); // 关闭显示
    OLED_WriteCmd(0xD5); OLED_WriteCmd(0x80); // 设置时钟分频
    OLED_WriteCmd(0xA8); OLED_WriteCmd(0x3F); // 设置驱动路数 (64行)
    OLED_WriteCmd(0xD3); OLED_WriteCmd(0x00); // 设置显示偏移
    OLED_WriteCmd(0x40); // 起始行
    OLED_WriteCmd(0x8D); OLED_WriteCmd(0x14); // 开启电荷泵
    OLED_WriteCmd(0x20); OLED_WriteCmd(0x02); // 页寻址模式（配合下面的 0xB0 页地址）
    OLED_WriteCmd(0xA1); // 段重映射
    OLED_WriteCmd(0xC8); // 整体重映射
    OLED_WriteCmd(0xDA); OLED_WriteCmd(0x12); // 设置COM引脚硬件配置
    OLED_WriteCmd(0x81); OLED_WriteCmd(0xCF); // 设置对比度
    OLED_WriteCmd(0xD9); OLED_WriteCmd(0xF1); // 设置预充电周期
    OLED_WriteCmd(0xDB); OLED_WriteCmd(0x40); // 设置VCOMH
    OLED_WriteCmd(0xA4); // 开启全局显示
    OLED_WriteCmd(0xA6); // 正常显示 (非反色)
    OLED_WriteCmd(0xAF); // 开启显示

    OLED_Clear();
    OLED_Refresh();
}

void OLED_Clear(void) {
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 128; j++) {
            OLED_GRAM[i][j] = 0x00;
        }
    }
}

// 将显存一次性刷入屏幕：每页一次 I2C 传输（控制字节 + 128 字节）
void OLED_Refresh(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_WriteCmd(0xB0 + page);              // 设置页地址 (0~7)
        OLED_WriteCmd(0x00);                     // 设置列低地址
        OLED_WriteCmd(0x10);                     // 设置列高地址

        oled_bus->write(OLED_ADDR, 0x40, &OLED_GRAM[page][0], OLED_WIDTH);
    }
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;

    if (color) {
        OLED_GRAM[y / 8][x] |= (1 << (y % 8));
    }
    else {
        OLED_GRAM[y / 8][x] &= ~(1 << (y % 8));
    }
}

// 显示字符（当前字库只有 8x16，y 需为 8 的倍数）
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size) {
    (void)size;
    if (chr < ' ' || chr > '~') {
        chr = ' ';
    }
    if (x > OLED_WIDTH - 8 || y > OLED_HEIGHT - 16) {
        return;
    }

    uint8_t page = y / 8;
    uint8_t c = (uint8_t)(chr - ' ');

    for (uint8_t i = 0; i < 8; i++) {
        OLED_GRAM[page][x + i]     = OLED_F8x16[c][i];
        OLED_GRAM[page + 1][x + i] = OLED_F8x16[c][i + 8];
    }
}

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size) {
    while (*str != '\0') {
        OLED_ShowChar(x, y, *str, size);
        x += size / 2; // 假设字体宽度是高度的一半 (8x16字体宽为8)
        if (x > 128 - size / 2) {
            x = 0;
            y += size;
        }
        str++;
    }
}