//
// Created by ZBright on 2026/11/12.
//

#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include "stm32f10x.h"

#define SYS_TIMx            TIM3
#define SYS_TIM_RCC         RCC_APB1Periph_TIM3
#define SYS_TIM_IRQn        TIM3_IRQn
#define SYS_TIM_IRQHandler  TIM3_IRQHandler

void BSP_Timer_Init(void);

#endif //BSP_TIMER_H
