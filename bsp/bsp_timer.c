//
// Created by ZBright on 2026/11/12.
//

#include "bsp_timer.h"

void BSP_Timer_Init(void) {
    RCC_APB1PeriphClockCmd(SYS_TIM_RCC, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure = {
        .TIM_Prescaler = TIM_CKD_DIV1,
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Prescaler = 7200 - 1,              // -> 10 KHz -> 0.1ms
        .TIM_Period = 10 - 1,
        .TIM_RepetitionCounter = 0,
    };
    TIM_TimeBaseInit(SYS_TIMx, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(SYS_TIMx, TIM_FLAG_Update);
    TIM_ITConfig(SYS_TIMx, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel = SYS_TIM_IRQn,			//选择配置NVIC线
        .NVIC_IRQChannelCmd = ENABLE,				//指定NVIC线路使能
        .NVIC_IRQChannelPreemptionPriority = 1,	    //指定NVIC线路的抢占优先级
        .NVIC_IRQChannelSubPriority = 0,			//指定NVIC线路的响应优先级
    };
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(SYS_TIMx, ENABLE);
}

/** 复制到main或IT文件

void SYS_TIM_IRQHandler(void) {

    if (TIM_GetITStatus(SYS_TIMx, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(SYS_TIMx, TIM_IT_Update);
        // 1ms 心跳业务

    }
}

 */
