# This file can be dropped into a project to help locate the Pico ESP01 Driver libraries
# It will also set up the required include and module search paths.
# Based on the Pimoroni Pico library by Pimoroni

if (DEFINED ENV{PICO_ESP01_DRIVER_FETCH_FROM_GIT} AND (NOT PICO_ESP01_DRIVER_FETCH_FROM_GIT))
    set(PICO_ESP01_DRIVER_FETCH_FROM_GIT $ENV{PICO_ESP01_DRIVER_FETCH_FROM_GIT})
    message("Using PICO_ESP01_DRIVER_FETCH_FROM_GIT from environment ('${PICO_ESP01_DRIVER_FETCH_FROM_GIT}')")
endif ()

if (DEFINED ENV{PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH} AND (NOT PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH))
    set(PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH $ENV{PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH})
    message("Using PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH from environment ('${PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH}')")
endif ()

if (NOT PICO_ESP01_DRIVER_PATH)
    if (PICO_ESP01_DRIVER_FETCH_FROM_GIT)
        include(FetchContent)
        set(FETCHCONTENT_BASE_DIR_SAVE ${FETCHCONTENT_BASE_DIR})
        if (PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH)
            get_filename_component(FETCHCONTENT_BASE_DIR "${PICO_ESP01_DRIVER_FETCH_FROM_GIT_PATH}" REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
        endif ()
        FetchContent_Declare(
                PICO_ESP01
                GIT_REPOSITORY https://github.com/deleranax/pico-esp01-driver
                GIT_TAG main
        )
        if (NOT PICO_ESP01)
            message("Downloading PICO_ESP01 SDK")
            FetchContent_Populate(PICO_ESP01)
            set(PICO_ESP01_DRIVER_PATH ${PICO_ESP01_DRIVER_SOURCE_DIR})
        endif ()
        set(FETCHCONTENT_BASE_DIR ${FETCHCONTENT_BASE_DIR_SAVE})
    elseif(PICO_SDK_PATH AND EXISTS "${PICO_SDK_PATH}/../pico-esp01-driver")
        set(PICO_ESP01_DRIVER_PATH ${PICO_SDK_PATH}/../pico-esp01-driver)
        message("Defaulting PICO_ESP01_DRIVER_PATH as sibling of PICO_SDK_PATH: ${PICO_ESP01_DRIVER_PATH}")
    elseif(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/../../pico-esp01-driver/")
        set(PICO_ESP01_DRIVER_PATH ${CMAKE_CURRENT_BINARY_DIR}/../../pico-esp01-driver/)
    else()
        message(FATAL_ERROR "Pico ESP01 Driver location was not specified. Please set PICO_ESP01_DRIVER_PATH.")
    endif()
endif()

get_filename_component(PICO_ESP01_DRIVER_PATH "${PICO_ESP01_DRIVER_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${PICO_ESP01_DRIVER_PATH})
    message(FATAL_ERROR "Directory '${PICO_ESP01_DRIVER_PATH}' not found")
endif ()

if (NOT EXISTS ${PICO_ESP01_DRIVER_PATH}/pico_esp01_driver_import.cmake)
    message(FATAL_ERROR "Directory '${PICO_ESP01_DRIVER_PATH}' does not appear to contain the Pico ESP01 Driver libraries")
endif ()

message("PICO_ESP01_DRIVER_PATH is ${PICO_ESP01_DRIVER_PATH}")

set(PICO_ESP01_DRIVER_PATH ${PICO_ESP01_DRIVER_PATH} CACHE PATH "Path to the Pico ESP01 Driver libraries" FORCE)

list(APPEND CMAKE_MODULE_PATH ${PICO_ESP01_DRIVER_PATH})

macro(pico_esp01_driver_init)
    add_subdirectory(${PICO_ESP01_DRIVER_PATH} esp01-driver)
endmacro()