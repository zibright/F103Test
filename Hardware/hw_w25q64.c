//
// Created by ZBright on 2026/11/23.
//

#include "hw_w25q64.h"
#include <stddef.h>

static SPI_Ops_t* flash_bus = NULL;

void W25Q64_Init(SPI_Ops_t* bus) {
    flash_bus = bus;
    bus->init();
}

/**
 * @brief 读取W25Q64的ID
 * @param mid 制造ID
 * @param did 设备ID
 * @return void
 */
void W25Q64_ReadID(uint8_t* mid, uint16_t* did) {
    flash_bus->start();
    flash_bus->swapByte(W25Q64_JEDEC_ID);
    *mid = flash_bus->swapByte(W25Q64_DUMMY_BYTE);
    *did = flash_bus->swapByte(W25Q64_DUMMY_BYTE);
    *did <<= 8;
    *did |= flash_bus->swapByte(W25Q64_DUMMY_BYTE);
    flash_bus->stop();
}

static void W25Q64_WriteEnable() {
    flash_bus->start();
    flash_bus->swapByte(W25Q64_WRITE_ENABLE);
    flash_bus->stop();
}

static void W25Q64_WaitBusy() {
    uint32_t timeout = 10000;
    flash_bus->start();
    flash_bus->swapByte(W25Q64_READ_STATUS_REGISTER_1);
    while ((flash_bus->swapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) {
        timeout--;
        if (timeout == 0) {
            break;
        }
    }
    flash_bus->stop();
}

/**
 * @brief 页面页编程(256Byte 不可跨页写入)
 * @param Address 页面页地址
 * @param DataArray 数据数组
 * @param DataCount 数据长度
 * @return void
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t* DataArray, uint16_t DataCount) {
    W25Q64_WriteEnable();
    flash_bus->start();
    flash_bus->swapByte(W25Q64_PAGE_PROGRAM);
    flash_bus->swapByte(Address >> 16);
    flash_bus->swapByte(Address >> 8);
    flash_bus->swapByte(Address);
    for (uint16_t i = 0; i < DataCount; i++) {
        flash_bus->swapByte(DataArray[i]);
    }
    flash_bus->stop();
    W25Q64_WaitBusy();
}

/**
 * @brief 扇区擦除(4KB -> 16页*256Byte)
 * @note 扇区地址必须是4KB的倍数
 * @param Address 扇区地址
 * @return void
 */
void W25Q64_SectorErase(uint32_t Address) {
    W25Q64_WriteEnable();
    flash_bus->start();
    flash_bus->swapByte(W25Q64_SECTOR_ERASE_4KB);
    flash_bus->swapByte(Address >> 16);
    flash_bus->swapByte(Address >> 8);
    flash_bus->swapByte(Address);
    flash_bus->stop();
    W25Q64_WaitBusy();
}

/**
 * @brief 读取数据(可跨页读取)
 * @param Address 数据地址
 * @param DataArray 数据数组
 * @param DataCount 数据长度
 * @return void
 */
void W25Q64_ReadData(uint32_t Address, uint8_t* DataArray, uint32_t DataCount) {
    flash_bus->start();
    flash_bus->swapByte(W25Q64_READ_DATA);
    flash_bus->swapByte(Address >> 16);
    flash_bus->swapByte(Address >> 8);
    flash_bus->swapByte(Address);
    for (uint32_t i = 0; i < DataCount; i++) {
        DataArray[i] = flash_bus->swapByte(W25Q64_DUMMY_BYTE);
    }
    flash_bus->stop();
}

/**
 * @brief 带有自动擦除功能的连续写入（大文件写入终极版）
 * @param Address 起始写入地址
 * @param DataArray 数据数组指针
 * @param DataCount 要写入的总字节数
 * @note ★ 极度安全警告：此函数会擦除包含起始地址和结束地址的整个扇区！
 *       如果是覆盖写入，请确保该扇区没有其他需要保留的数据。
 */
void W25Q64_WriteData(uint32_t Address, uint8_t* DataArray, uint32_t DataCount) {
    if (DataCount == 0) return;

    // 1. 计算需要擦除的起始扇区和结束扇区
    // W25Q64 的一个扇区是 4096 字节 (0x1000)
    uint32_t start_sector = Address / 4096;
    uint32_t end_sector = (Address + DataCount - 1) / 4096;

    // 2. 循环擦除涉及的扇区
    for (uint32_t sector = start_sector; sector <= end_sector; sector++) {
        uint32_t sector_addr = sector * 4096;
        W25Q64_SectorErase(sector_addr);
    }

    // 3. 执行跨页连续写入
    uint32_t offset = 0;
    while (offset < DataCount) {
        // 计算当前页剩余空间
        uint16_t page_remain = 256 - (Address % 256);
        // 计算本次写入长度
        uint16_t write_size = (DataCount - offset) < page_remain ? (DataCount - offset) : page_remain;

        // 调用底层页编程
        W25Q64_PageProgram(Address, DataArray + offset, write_size);

        // 更新地址和偏移量
        Address += write_size;
        offset += write_size;
    }
}