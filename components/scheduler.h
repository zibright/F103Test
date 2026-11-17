//
// Created by ZBright on 2026/11/12.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 10    // 最大任务数

typedef enum {
    TASK_DISABLE = 0,
    TASK_ENABLE = 1
}TaskState_t;

typedef struct {
    void (*task_func)(void); // 任务函数指针
    uint16_t period;         // 任务周期 (单位: ms)
    uint16_t timer;          // 倒计时器 (单位: ms)
    TaskState_t state;       // 任务使能状态
}Task_t;

void    Scheduler_Init(void);
int8_t  Scheduler_AddTask(void (*task_func)(void), uint16_t period);
void    Scheduler_EnableTask(uint8_t task_id, uint8_t enable);
void    Scheduler_Tick(void);
void    Scheduler_Run(void);

#endif //SCHEDULER_H
