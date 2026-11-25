//
// Created by ZBright on 2026/11/23.
//

#include "bsp_spi.h"
#include "stm32f10x.h"

#define SOFT_SPI_SS0(BitValue)  GPIO_WriteBit(SOFT_SPI_SS0_PORT, SOFT_SPI_SS0_PIN,   (BitAction)BitValue)
#define SOFT_SPI_SCLK(BitValue) GPIO_WriteBit(SOFT_SPI_SCLK_PORT, SOFT_SPI_SCLK_PIN, (BitAction)BitValue)
#define SOFT_SPI_MOSI(BitValue) GPIO_WriteBit(SOFT_SPI_MOSI_PORT, SOFT_SPI_MOSI_PIN, (BitAction)BitValue)
#define SOFT_SPI_MISO()         GPIO_ReadInputDataBit(SOFT_SPI_MISO_PORT, SOFT_SPI_MISO_PIN)

static void Soft_SPI_Init(void) {
    RCC_APB2PeriphClockCmd(SOFT_SPI_SS0_CLK | SOFT_SPI_SCLK_CLK | SOFT_SPI_MISO_CLK | SOFT_SPI_MOSI_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_SS0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_SPI_SS0_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_SCLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_SPI_SCLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_SPI_MOSI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SOFT_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SOFT_SPI_MISO_PORT, &GPIO_InitStructure);

    SOFT_SPI_SS0(1);
    SOFT_SPI_SCLK(0);
}

static void SPI_Start(void) {
    SOFT_SPI_SS0(0);
}

static void SPI_Stop(void) {
    SOFT_SPI_SS0(1);
}

static uint8_t  SPI_SwapByte(uint8_t ByteSend) {
    /**掩码交换字节****************************
    uint8_t ByteRecv = 0x00;
    for (int i = 0; i < 8; i++) {
        // SOFT_SPI_MOSI((ByteSend & (0x80 >> i)) ? 1 : 0); // 三目运算符写法
        SOFT_SPI_MOSI(!!(ByteSend & (0x80 >> i)));          // 逻辑非运算符写法

        SOFT_SPI_SCLK(1);
        if (SOFT_SPI_MISO() == Bit_SET) {
            ByteRecv |= (0x80 >> i);
        }
        SOFT_SPI_SCLK(0);
    }
    return ByteRecv;
    *******************************掩码交换字节**/

    /**移位数据实现****************************/
    for (int i = 0; i < 8; i++) {
        SOFT_SPI_MOSI(!!(ByteSend & 0x80));
        ByteSend <<= 1;
        SOFT_SPI_SCLK(1);
        if (SOFT_SPI_MISO() == Bit_SET) {
            ByteSend |= 0x01;
        }
        SOFT_SPI_SCLK(0);
    }
    return ByteSend;
    /*******************************移位数据实现**/
}

SPI_Ops_t spi_soft = {
    Soft_SPI_Init,
    SPI_Start,
    SPI_SwapByte,
    SPI_Stop
};
