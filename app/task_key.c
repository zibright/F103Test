#include "task_key.h"

KeyTestData_t g_key_test;

static uint8_t next_count(uint8_t v)
{
    return (uint8_t)((v + 1U) % 100U);
}

void Task_Key_Init(void)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        g_key_test.key[i].level  = 1;
        g_key_test.key[i].hold   = 0;
        g_key_test.key[i].down   = 0;
        g_key_test.key[i].up     = 0;
        g_key_test.key[i].single = 0;
        g_key_test.key[i].dbl    = 0;
        g_key_test.key[i].lng    = 0;
        g_key_test.key[i].rpt    = 0;
        g_key_test.key[i].last   = 0;
    }
    g_key_test.selected = KEY_1;
}

void Task_Key_Poll(void)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        KeyTestStat_t *s = &g_key_test.key[i];

        s->level = (uint8_t)(BSP_Key_Read(i) ? 1U : 0U);
        s->hold  = (uint8_t)(HW_Key_Check(i, KEY_HOLD) ? 1U : 0U);

        if (HW_Key_Check(i, KEY_DOWN))   { s->down   = next_count(s->down);   s->last = 1; g_key_test.selected = i; }
        if (HW_Key_Check(i, KEY_UP))     { s->up     = next_count(s->up);     s->last = 2; }
        if (HW_Key_Check(i, KEY_SINGLE)) { s->single = next_count(s->single); s->last = 3; }
        if (HW_Key_Check(i, KEY_DOUBLE)) { s->dbl    = next_count(s->dbl);    s->last = 4; }
        if (HW_Key_Check(i, KEY_LONG))   { s->lng    = next_count(s->lng);    s->last = 5; }
        if (HW_Key_Check(i, KEY_REPEAT)) { s->rpt    = next_count(s->rpt);    s->last = 6; }
    }
}