//
// Created by ZBright on 2026/11/11.
//

#ifndef OLED_FONTS_H
#define OLED_FONTS_H

#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			(3)		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
    uint8_t Data[60];						//字模数据
} ChineseCell20x20_t;
typedef struct
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
    uint8_t Data[32];						//字模数据
} ChineseCell16x16_t;
typedef struct
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // 汉字索引
    uint8_t Data[24];                      // 字模数据，12*12的汉字需要24字节
} ChineseCell12x12_t;

typedef struct
{
    char Index[OLED_CHN_CHAR_WIDTH + 1];   // 汉字索引
    uint8_t Data[8];                      // 字模数据，8*8的汉字需要24字节
} ChineseCell8x8_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];

/*汉字字模数据声明*/
extern const ChineseCell16x16_t OLED_CF16x16[];

#endif //OLED_FONTS_H
