cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Core project settings
project(Project LANGUAGES C CXX ASM)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 1. 可执行文件：入口、中断、系统调用、启动文件
add_executable(${PROJECT_NAME}
    Core/Src/main.c
    Core/Src/stm32f10x_it.c
    Core/Src/syscalls.c
    startup_stm32f103xb.s
)

# 2. 厂商库（提供头文件、宏和驱动对象）
add_subdirectory(cmake/stdperiph_lib)

# 3. 模块静态库（顺序：被依赖者在前）
add_subdirectory(bsp)
add_subdirectory(Hardware)
add_subdirectory(components)
add_subdirectory(app)

# Link libraries
# include 路径由各模块 target 的 PUBLIC 属性传递：
#   bsp/      -> "bsp_i2c.h"、"i2c/bsp_i2c.h"
#   Hardware/ -> "oled/hw_oled.h"
target_link_libraries(${PROJECT_NAME} PRIVATE
    app
    hardware
    components
    bsp

    STM32_SPL_Drivers
    stdperiph_lib

    m
)

# Generate .hex and .bin next to the .elf for flashing tools
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Generating ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin"
)
