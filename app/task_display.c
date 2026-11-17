#include <stdio.h>
#include "task_display.h"
#include "task_key.h"
#include "oled/hw_oled.h"

static const char *event_name(uint8_t ev)
{
    switch (ev) {
        case 1: return "DOWN";
        case 2: return "UP";
        case 3: return "SINGLE";
        case 4: return "DOUBLE";
        case 5: return "LONG";
        case 6: return "REPEAT";
        default: return "NONE";
    }
}

static void show_key_test(void)
{
    char line[24];
    uint8_t i = g_key_test.selected;
    const KeyTestStat_t *s = &g_key_test.key[i];

    OLED_Clear();

    snprintf(line, sizeof(line), "K%u LV:%u H:%u",
             (unsigned)(i + 1U),
             (unsigned)s->level,
             (unsigned)s->hold);
    OLED_ShowString(0, 0, line, 16);

    snprintf(line, sizeof(line), "DN:%02u U:%02u S:%02u",
             (unsigned)(s->down % 100U),
             (unsigned)(s->up % 100U),
             (unsigned)(s->single % 100U));
    OLED_ShowString(0, 16, line, 16);

    snprintf(line, sizeof(line), "DB:%02u LG:%02u R:%02u",
             (unsigned)(s->dbl % 100U),
             (unsigned)(s->lng % 100U),
             (unsigned)(s->rpt % 100U));
    OLED_ShowString(0, 32, line, 16);

    snprintf(line, sizeof(line), "EVT:%s", event_name(s->last));
    OLED_ShowString(0, 48, line, 16);

    OLED_Refresh();
}

void Task_Display_Init(void)
{
    show_key_test();
}

void Task_Display_Poll(void)
{
    show_key_test();
}