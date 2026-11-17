#include "stm32f10x.h"
#include "bsp_delay.h"

void delay_us(uint32_t us)
{
    uint32_t ticks = (SystemCoreClock / 1000000U) * us;

    if (ticks == 0U)
    {
        ticks = 1U;
    }
    if (ticks > 0xFFFFFFU)
    {
        ticks = 0xFFFFFFU;
    }

    SysTick->LOAD = ticks - 1U;
    SysTick->VAL  = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U)
    {
    }

    SysTick->CTRL = 0U;
}

void delay_ms(uint32_t ms)
{
    while (ms-- > 0U)
    {
        delay_us(1000U);
    }
}