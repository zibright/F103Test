#ifndef HW_KEY_H
#define HW_KEY_H

#include "bsp_key.h"

/* ===== 按键事件标志（位掩码） ===== */
#define KEY_HOLD				0x01  // 持续按下（不清除）
#define KEY_DOWN				0x02  // 按下瞬间
#define KEY_UP					0x04  // 松开瞬间
#define KEY_SINGLE				0x08  // 短按（单击）
#define KEY_DOUBLE				0x10  // 双击
#define KEY_LONG				0x20  // 长按触发
#define KEY_REPEAT				0x40  // 连按触发

void HW_Key_Init(void);                                /* 按键初始化 */
uint8_t HW_Key_Check(uint8_t n, uint8_t Flag);         /* 查询并取走按键事件 */
void HW_Key_Tick(void);                                /* 1ms 中断调用：状态机扫描 */

#endif // HW_KEY_H