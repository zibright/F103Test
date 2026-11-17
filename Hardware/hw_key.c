#include "hw_key.h"

// ===== 时间参数配置（单位：ms） =====
#define KEY_TIME_DOUBLE			150   // 双击判定间隔
#define KEY_TIME_LONG			1000  // 长按判定时间
#define KEY_TIME_REPEAT			100   // 连按触发间隔

#define KEY_PRESSED				0     // 上拉输入，按下为 0
#define KEY_UNPRESSED			1     // 未按下为 1

// 事件标志缓存
static uint8_t Key_Flag[KEY_COUNT];

void HW_Key_Init(void)
{
    BSP_Key_Init();

    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        Key_Flag[i] = 0;
    }
}

// 查询并取走事件
uint8_t HW_Key_Check(uint8_t n, uint8_t Flag)
{
    if (Key_Flag[n] & Flag)
    {
        // KEY_HOLD 是实时状态，不需要清除；其他事件读取后自动清除
        if (Flag != KEY_HOLD)
        {
            Key_Flag[n] &= ~Flag;
        }
        return 1;
    }
    return 0;
}

// 状态机（需放在 1ms 定时器中断中）
void HW_Key_Tick(void)
{
    static uint8_t Count = 0, i;
    static uint8_t CurrState[KEY_COUNT], PrevState[KEY_COUNT];
    static uint8_t S[KEY_COUNT];
    static uint16_t Time[KEY_COUNT];

    // 1. 时间递减
    for (i = 0; i < KEY_COUNT; i++)
    {
        if (Time[i] > 0)
        {
            Time[i]--;
        }
    }

    // 2. 每 20ms 采样一次
    Count++;
    if (Count >= 20)
    {
        Count = 0;

        for (i = 0; i < KEY_COUNT; i++)
        {
            // 读取当前电平（注意：原版 1 为按下，我们改为 0 为按下）
            uint8_t raw_level = BSP_Key_Read(i);
            CurrState[i] = (raw_level == 0) ? KEY_PRESSED : KEY_UNPRESSED;

            // 更新持续按下状态
            if (CurrState[i] == KEY_PRESSED)
            {
                Key_Flag[i] |= KEY_HOLD;
            }
            else
            {
                Key_Flag[i] &= ~KEY_HOLD;
            }

            // 按下瞬间（下降沿）
            if (CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
            {
                Key_Flag[i] |= KEY_DOWN;
            }

            // 松开瞬间（上升沿）
            if (CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
            {
                Key_Flag[i] |= KEY_UP;
            }

            PrevState[i] = CurrState[i];

            // ===== 状态机核心逻辑 =====
            if (S[i] == 0) // 空闲状态
            {
                if (CurrState[i] == KEY_PRESSED)
                {
                    Time[i] = KEY_TIME_LONG;
                    S[i] = 1;
                }
            }
            else if (S[i] == 1) // 已按下，等待长按或松开
            {
                if (CurrState[i] == KEY_UNPRESSED) // 松手了，等待双击判定
                {
                    Time[i] = KEY_TIME_DOUBLE;
                    S[i] = 2;
                }
                else if (Time[i] == 0) // 按住达到了长按时间
                {
                    Time[i] = KEY_TIME_REPEAT;
                    Key_Flag[i] |= KEY_LONG;
                    S[i] = 4;
                }
            }
            else if (S[i] == 2) // 松开状态，等待双击
            {
                if (CurrState[i] == KEY_PRESSED) // 双击了！
                {
                    Key_Flag[i] |= KEY_DOUBLE;
                    S[i] = 3;
                }
                else if (Time[i] == 0) // 超时没按下，确认为单击
                {
                    Key_Flag[i] |= KEY_SINGLE;
                    S[i] = 0;
                }
            }
            else if (S[i] == 3) // 双击后的等待松开
            {
                if (CurrState[i] == KEY_UNPRESSED)
                {
                    S[i] = 0;
                }
            }
            else if (S[i] == 4) // 长按中
            {
                if (CurrState[i] == KEY_UNPRESSED) // 松手了
                {
                    S[i] = 0;
                }
                else if (Time[i] == 0) // 连按触发
                {
                    Time[i] = KEY_TIME_REPEAT;
                    Key_Flag[i] |= KEY_REPEAT;
                    S[i] = 4;
                }
            }
        }
    }
}