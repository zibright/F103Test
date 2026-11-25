//
// Created by ZBright on 2026/11/25.
//

#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

// 计算一段数据的 CRC32
uint32_t CRC32_Calculate(const uint8_t *data, uint32_t length);

// 分段计算（用于边接收边计算）
uint32_t CRC32_Update(uint32_t current_crc, const uint8_t *data, uint32_t length);

#endif //CRC32_H
