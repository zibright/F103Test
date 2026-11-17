# App 层：业务任务（当前没有 .c 文件，先作为接口库）
# 自动收集本模块（含子目录）的所有 .c 文件
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

if(APP_SOURCES)
    add_library(app STATIC ${APP_SOURCES})
    target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(app PUBLIC
        hardware
        components
        bsp
        stdperiph_lib
    )
else()
    add_library(app INTERFACE)
    target_include_directories(app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(app INTERFACE
        hardware
        components
        bsp
        stdperiph_lib
    )
endif()