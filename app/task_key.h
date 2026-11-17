#ifndef TASK_KEY_H
#define TASK_KEY_H

#include <stdint.h>
#include "hw_key.h"

/* 单个按键的测试数据 */
typedef struct {
    uint8_t level;   // 原始电平：0 按下，1 松开
    uint8_t hold;    // 当前是否按住
    uint8_t down;    // 按下次数
    uint8_t up;      // 松开次数
    uint8_t single;  // 短按次数
    uint8_t dbl;     // 双击次数
    uint8_t lng;     // 长按次数
    uint8_t rpt;     // 连发次数
    uint8_t last;    // 最近事件
} KeyTestStat_t;

/* 4 个按键的测试数据 + 当前查看的按键 */
typedef struct {
    KeyTestStat_t key[KEY_COUNT];
    uint8_t selected;
} KeyTestData_t;

extern KeyTestData_t g_key_test;

void Task_Key_Init(void);
void Task_Key_Poll(void);   // 建议 10ms 周期

#endif //TASK_KEY_H