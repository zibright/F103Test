//
// Created by ZBright on 2026/11/23.
//

#ifndef BSP_SPI_H
#define BSP_SPI_H

#include <stdint.h>

/** Soft SPI GPIO Define ******************/
#define SOFT_SPI_SS0_CLK        RCC_APB2Periph_GPIOB
#define SOFT_SPI_SS0_PORT       GPIOB
#define SOFT_SPI_SS0_PIN        GPIO_Pin_12

#define SOFT_SPI_SCLK_CLK       RCC_APB2Periph_GPIOB
#define SOFT_SPI_SCLK_PORT      GPIOB
#define SOFT_SPI_SCLK_PIN       GPIO_Pin_13

#define SOFT_SPI_MISO_CLK       RCC_APB2Periph_GPIOB
#define SOFT_SPI_MISO_PORT      GPIOB
#define SOFT_SPI_MISO_PIN       GPIO_Pin_14

#define SOFT_SPI_MOSI_CLK       RCC_APB2Periph_GPIOB
#define SOFT_SPI_MOSI_PORT      GPIOB
#define SOFT_SPI_MOSI_PIN       GPIO_Pin_15

/******************* Soft SPI GPIO Define **/

typedef struct {
    void (*init)(void);
    void (*start)(void);
    uint8_t (*swapByte)(uint8_t ByteSend);
    void (*stop)(void);
}SPI_Ops_t;

extern SPI_Ops_t spi_soft;

#endif //BSP_SPI_H
