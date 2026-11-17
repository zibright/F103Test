# 自动收集本模块（含子目录）的所有 .c 文件，新增/删除文件无需改 CMake
file(GLOB_RECURSE HARDWARE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(hardware STATIC ${HARDWARE_SOURCES})

# 只暴露模块组根目录，外部统一写 "oled/hw_oled.h"
target_include_directories(hardware PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(hardware PUBLIC
    bsp
    stdperiph_lib
)