cmake_minimum_required(VERSION 3.22)

# 启用 C 和 ASM 支持
enable_language(C ASM)

# 1. 定义宏 (根据芯片修改)
set(SPL_Defines_Syms 
    USE_STDPERIPH_DRIVER 
    STM32F10X_MD             # 核心宏：根据芯片容量改为 LD, MD, HD
    $<$<CONFIG:Debug>:DEBUG>
)

# 2. 指定头文件路径
set(SPL_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F10x_StdPeriph_Driver/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS
)

# 3. 应用源码由顶层 CMakeLists 的 add_executable 统一管理

# 4. 搜集标准库驱动 (Driver Sources)
# CONFIGURE_DEPENDS: 新增/删除 .c 文件后自动重新配置
file(GLOB SPL_Drivers_Src CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32F10x_StdPeriph_Driver/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/*.c # 包含 core_cm3.c 等
)

# --- 以下是构建“魔法”，模仿了 CubeMX 的结构 ---

# 5. 定义接口库 (管理头文件和宏)
add_library(stdperiph_lib INTERFACE)
target_include_directories(stdperiph_lib INTERFACE ${SPL_Include_Dirs})
target_compile_definitions(stdperiph_lib INTERFACE ${SPL_Defines_Syms})

# 6. 编译驱动静态库 (加快后续编译速度)
add_library(STM32_SPL_Drivers OBJECT)
target_sources(STM32_SPL_Drivers PRIVATE ${SPL_Drivers_Src})
target_link_libraries(STM32_SPL_Drivers PUBLIC stdperiph_lib)

# 7. 设置清理规则 (生成 .map 文件)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)
