# Components 层：纯软件组件（当前没有 .c 文件，先作为接口库）
# 自动收集本模块（含子目录）的所有 .c 文件
file(GLOB_RECURSE COMPONENT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

if(COMPONENT_SOURCES)
    add_library(components STATIC ${COMPONENT_SOURCES})
    target_include_directories(components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
else()
    add_library(components INTERFACE)
    target_include_directories(components INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Components 是纯软件层，默认不依赖 STM32 标准库。
# 以后加入 OLED_UI 等需要显示的组件时，再按需链接 hardware：
# target_link_libraries(components PUBLIC hardware)