# 自动收集本模块（含子目录）的所有 .c 文件，新增/删除文件无需改 CMake
file(GLOB_RECURSE BSP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_library(bsp STATIC ${BSP_SOURCES})

# 暴露 bsp/ 作为 include 根：
#   bsp/xxx.h      -> "xxx.h"
#   bsp/i2c/xxx.h  -> "i2c/xxx.h"
target_include_directories(bsp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bsp PUBLIC
    stdperiph_lib
)