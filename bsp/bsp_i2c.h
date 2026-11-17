//
// Created by ZBright on 2026/11/11.
//

#ifndef BSP_I2C_H
#define BSP_I2C_H

#include <stdint.h>

/** Soft I2C ******************/
#define SOFT_I2C_SCL_CLK      RCC_APB2Periph_GPIOB
#define SOFT_I2C_SCL_PORT     GPIOB
#define SOFT_I2C_SCL_PIN      GPIO_Pin_8

#define SOFT_I2C_SDA_CLK      RCC_APB2Periph_GPIOB
#define SOFT_I2C_SDA_PORT     GPIOB
#define SOFT_I2C_SDA_PIN      GPIO_Pin_9

/******************* Soft I2C **/

/** Hard I2C1 *******************/
#define HW1_I2C_CLK       RCC_APB2Periph_GPIOB
#define HW1_I2C_CLK_CMD   RCC_APB1Periph_I2C1
#define HW1_I2C_PERIPH    I2C1
#define HW1_I2C_PORT      GPIOB
#define HW1_I2C_SCL_PIN   GPIO_Pin_6
#define HW1_I2C_SDA_PIN   GPIO_Pin_7

/******************* Hard I2C1 **/
typedef struct{
  void (*init)  (void);
  void (*write) (uint8_t devAddr, uint8_t reg, uint8_t* data, uint16_t len);
  void (*read)  (uint8_t devAddr, uint8_t reg, uint8_t* data, uint16_t len);
}I2C_Ops_t;

extern I2C_Ops_t i2c_soft;
extern I2C_Ops_t i2c_hw1;
extern I2C_Ops_t i2c_hw2;

#endif //BSP_I2C_H
