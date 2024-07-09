cmake_minimum_required(VERSION 3.28)

# Pull in SDK
include(pico_sdk_import.cmake)

project(pico_esp01_driver C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(SRC_FILES ${SRC_DIR}/esp01.c ${SRC_DIR}/esp01.h)

# Initialize the SDK
pico_sdk_init()

add_library(esp01 ${SRC_FILES})

target_include_directories(esp01 PUBLIC ${SRC_DIR})

target_link_libraries(esp01 pico_stdlib)