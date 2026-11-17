//
// Created by ZBright on 2026/11/12.
//

#include "scheduler.h"
#include <stddef.h>

static Task_t task_list[MAX_TASKS];
static uint8_t task_count = 0;

void Scheduler_Init(void) {
    task_count = 0;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        task_list[i].task_func = NULL;
        task_list[i].period = 0;
        task_list[i].timer = 0;
        task_list[i].state = TASK_DISABLE;
    }
}

int8_t Scheduler_AddTask(void (*task_func)(void), uint16_t period) {
    if (task_count >= MAX_TASKS || task_func == NULL || period == 0) {
        return -1;
    }

    uint8_t id = task_count;
    task_list[id].task_func = task_func;
    task_list[id].period = period;
    task_list[id].timer = period;
    task_list[id].state = TASK_ENABLE;

    task_count++;
    return id;
}

void Scheduler_EnableTask(uint8_t task_id, uint8_t enable) {
    if (task_id < task_count) {
        task_list[task_id].state = enable?TASK_ENABLE:TASK_DISABLE;
    }
}

void Scheduler_Tick(void) {
    for (uint8_t i = 0; i < task_count; i++) {
        if (task_list[i].state == TASK_ENABLE) {
            if (task_list[i].timer > 0) {
                task_list[i].timer--;
            }
        }
    }
}

void Scheduler_Run(void) {
     for (uint8_t i = 0; i < task_count; i++) {
         if (task_list[i].state == TASK_ENABLE && task_list[i].timer == 0) {
             if (task_list[i].task_func != NULL) {
                 task_list[i].task_func();
             }

             task_list[i].timer = task_list[i].period;
         }
     }
}

