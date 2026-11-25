//
// Created by ZBright on 2026/11/23.
//

#ifndef HW_W25Q64_H
#define HW_W25Q64_H

#include <stdint.h>
#include "bsp_spi.h"

/** W25Q64 分区****************************************/
#define W25Q64_SECTOR_SIZE  4096
// 按块划分
#define PART_CONFIG_START   0x000000
#define PART_CONFIG_SIZE    (8 * 1024)

#define PART_OTA_START      0x002000
#define PART_OTA_SIZE       (1024 * 1024)

#define PART_ASSETS_START   0x102000
#define PART_ASSETS_SIZE    (2 * 1024 * 1024)

#define PART_LOG_START      0x302000
#define PART_LOG_SIZE       (5 * 1024 * 1024)
/************************************** W25Q64 分区 **/

/** W25Q64 指令集 ****************************************/
#define W25Q64_WRITE_ENABLE							0x06
#define W25Q64_WRITE_DISABLE						0x04
#define W25Q64_READ_STATUS_REGISTER_1				0x05
#define W25Q64_READ_STATUS_REGISTER_2				0x35
#define W25Q64_WRITE_STATUS_REGISTER				0x01
#define W25Q64_PAGE_PROGRAM							0x02
#define W25Q64_QUAD_PAGE_PROGRAM					0x32
#define W25Q64_BLOCK_ERASE_64KB						0xD8
#define W25Q64_BLOCK_ERASE_32KB						0x52
#define W25Q64_SECTOR_ERASE_4KB						0x20
#define W25Q64_CHIP_ERASE							0xC7
#define W25Q64_ERASE_SUSPEND						0x75
#define W25Q64_ERASE_RESUME							0x7A
#define W25Q64_POWER_DOWN							0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE				0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID				0x90
#define W25Q64_READ_UNIQUE_ID						0x4B
#define W25Q64_JEDEC_ID								0x9F
#define W25Q64_READ_DATA							0x03
#define W25Q64_FAST_READ							0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Q64_FAST_READ_DUAL_IO					0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Q64_FAST_READ_QUAD_IO					0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO				0xE3

#define W25Q64_DUMMY_BYTE							0xFF
/**************************************** W25Q64 指令集 **/

void W25Q64_Init(SPI_Ops_t* bus);
void W25Q64_ReadID(uint8_t* mid, uint16_t* did);
void W25Q64_PageProgram(uint32_t Address, uint8_t* DataArray, uint16_t DataCount);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t* DataArray, uint32_t DataCount);
void W25Q64_WriteData(uint32_t Address, uint8_t* DataArray, uint32_t DataCount);

#endif //HW_W25Q64_H
