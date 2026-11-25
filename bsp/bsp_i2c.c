//
// Created by ZBright on 2026/11/11.
//
#include "stm32f10x.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"

/** Soft I2C ******************************/
#define SOFT_I2C_SCL(BitValue)  GPIO_WriteBit(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN, (BitAction)BitValue)
#define SOFT_I2C_SDA(BitValue)  GPIO_WriteBit(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, (BitAction)BitValue)

static void Soft_I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(SOFT_I2C_SCL_CLK | SOFT_I2C_SDA_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SOFT_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SOFT_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(SOFT_I2C_SDA_PORT, &GPIO_InitStructure);

    SOFT_I2C_SCL(1);
    SOFT_I2C_SDA(1);
}

// --- 内部时序函数 ---
static void I2C_Start(void) {
    SOFT_I2C_SDA(1); SOFT_I2C_SCL(1); delay_us(2);
    SOFT_I2C_SDA(0);  delay_us(2);
    SOFT_I2C_SCL(0);
}

static void I2C_Stop(void) {
    SOFT_I2C_SDA(0);  delay_us(2);
    SOFT_I2C_SCL(1); delay_us(2);
    SOFT_I2C_SDA(1); delay_us(2);
}

static void I2C_SendByte(uint8_t dat) {
    for (uint8_t i = 0; i < 8; i++) {
        if (dat & 0x80) SOFT_I2C_SDA(1); else SOFT_I2C_SDA(0);
        delay_us(2);
        SOFT_I2C_SCL(1); delay_us(2);
        SOFT_I2C_SCL(0);  delay_us(2);
        dat <<= 1;
    }
    // 接收 ACK（省略详细逻辑，直接拉高拉低）
    SOFT_I2C_SDA(1); delay_us(2);
    SOFT_I2C_SCL(1); delay_us(2);
    SOFT_I2C_SCL(0);
}

static void Soft_I2C_Write(uint8_t devAddr, uint8_t reg, uint8_t* data,uint16_t len){
    I2C_Start();
    I2C_SendByte(devAddr & 0xFE);
    I2C_SendByte(reg);
    for (uint16_t i = 0; i < len; i++){
        I2C_SendByte(data[i]);
    }
    I2C_Stop();
}

static void Soft_I2C_Read(uint8_t devAddr, uint8_t reg, uint8_t* data, uint16_t len){

}

I2C_Ops_t i2c_soft = {
    Soft_I2C_Init,
    Soft_I2C_Write,
    Soft_I2C_Read,
};

/* ========================================================
   二、 硬件 I2C1 实现
   ======================================================== */
static void HW1_I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_APB2PeriphClockCmd(HW1_I2C_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(HW1_I2C_CLK_CMD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = HW1_I2C_SCL_PIN | HW1_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HW1_I2C_PORT, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x0A;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(HW1_I2C_PERIPH, &I2C_InitStructure);

    I2C_Cmd(HW1_I2C_PERIPH, ENABLE);
}

static void HW1_I2C_Write(uint8_t devAddr, uint8_t reg, uint8_t* data, uint16_t len) {
    // 简单防死锁等待（实际项目中建议加超时时间）
    while (I2C_GetFlagStatus(HW1_I2C_PERIPH, I2C_FLAG_BUSY));

    I2C_GenerateSTART(HW1_I2C_PERIPH, ENABLE);
    while (!I2C_CheckEvent(HW1_I2C_PERIPH, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(HW1_I2C_PERIPH, devAddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(HW1_I2C_PERIPH, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(HW1_I2C_PERIPH, reg);
    while (!I2C_CheckEvent(HW1_I2C_PERIPH, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

    for (uint16_t i = 0; i < len; i++) {
        I2C_SendData(HW1_I2C_PERIPH, data[i]);
        while (!I2C_CheckEvent(HW1_I2C_PERIPH, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    I2C_GenerateSTOP(HW1_I2C_PERIPH, ENABLE);
}

static void HW1_I2C_Read(uint8_t devAddr, uint8_t reg, uint8_t* data, uint16_t len) {
    // 暂不实现
}

// 实例化硬件 I2C1
I2C_Ops_t i2c_hw1 = { HW1_I2C_Init, HW1_I2C_Write, HW1_I2C_Read };