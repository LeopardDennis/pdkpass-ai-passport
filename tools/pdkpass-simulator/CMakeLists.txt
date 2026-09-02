cmake_minimum_required(VERSION 3.16)

if(NOT APPLE)
    message(FATAL_ERROR "The PDKPASS simulator currently requires macOS.")
endif()

project(pdkpass_simulator LANGUAGES C OBJC)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h"
    CACHE FILEPATH "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

get_filename_component(PDKPASS_REPO "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_subdirectory("${PDKPASS_REPO}/managed_components/lvgl__lvgl" lvgl)

add_executable(pdkpass-simulator
    simulator.m
    "${PDKPASS_REPO}/main/ui_pixel.c"
    "${PDKPASS_REPO}/main/ui_pixel_math.c"
    "${PDKPASS_REPO}/main/pdkpass_ui.c"
    "${PDKPASS_REPO}/main/pdkpass_model.c"
    "${PDKPASS_REPO}/main/pdkpass_results_core.c"
    "${PDKPASS_REPO}/main/pdkpass_schedule.c"
    "${PDKPASS_REPO}/main/pdkpass_data.c"
    "${PDKPASS_REPO}/main/pdkpass_theme.c"
    "${PDKPASS_REPO}/main/pdkpass_tracks.c"
)

target_include_directories(pdkpass-simulator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/stubs"
    "${PDKPASS_REPO}/main"
)
target_compile_options(pdkpass-simulator PRIVATE
    $<$<COMPILE_LANGUAGE:OBJC>:-fobjc-arc>
)

find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
find_library(UTTYPE_FRAMEWORK UniformTypeIdentifiers REQUIRED)
target_link_libraries(pdkpass-simulator PRIVATE
    lvgl::lvgl
    m
    "${COCOA_FRAMEWORK}"
    "${UTTYPE_FRAMEWORK}"
)
