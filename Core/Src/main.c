#include "stm32f10x.h"
#include "bsp_timer.h"
#include "scheduler.h"
#include "oled/hw_oled.h"
#include "hw_key.h"
#include "task_key.h"
#include "task_display.h"

int main(void) {
    SystemCoreClockUpdate();
    // 硬件初始化
    OLED_Init(&i2c_soft);
    HW_Key_Init();
    BSP_Timer_Init();
    // 任务初始化
    Task_Key_Init();
    Task_Display_Init();
    // 调度器初始化&任务注册
    Scheduler_Init();
    Scheduler_AddTask(Task_Key_Poll, 10);
    Scheduler_AddTask(Task_Display_Poll, 100);

    while (1) {
        Scheduler_Run();
    }
}

// 1ms 定时器心跳
void SYS_TIM_IRQHandler(void) {
    if (TIM_GetITStatus(SYS_TIMx, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(SYS_TIMx, TIM_IT_Update);
        // 驱动按键状态机
        HW_Key_Tick();
        // 驱动调度器
        Scheduler_Tick();
    }
}