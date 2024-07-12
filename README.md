# Pi Pico ESP01 Driver

## About

Simple Pi Pico driver for the C/C++ SDK to communicate with ESP01 Wifi module (using UART).

## Quick start

The driver can be imported using a cmake script (similar to the [`pico-sdk`](https://github.com/raspberrypi/pico-sdk/)).

Here is a simple quick start guide:

1. Setup the `pico-sdk` for your project
2. Clone this repository
3. Download `pico_esp01_driver_import.cmake` and put it along with `pico_sdk_import.cmake` (in the root folder of your
   project)
4. Copy and fill this `CMakeLists.txt` template

```cmake
cmake_minimum_required(VERSION 3.28)

set(PICO_SDK_PATH "YOUR/SDK/PATH")
#set(PICO_ESP01_DRIVER_PATH "YOUR/DRIVER/PATH")

# Pull in SDK
include(pico_sdk_import.cmake)
include(pico_esp01_driver_import.cmake)

project(test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()
pico_esp01_driver_init()

add_executable(test main.c)
target_link_libraries(test pico_stdlib esp01)
pico_add_extra_outputs(test)
```

If you cloned the driver in the same directory as the [`pico-sdk`](https://github.com/raspberrypi/pico-sdk/), you don't
need to set `PICO_ESP01_DRIVER_PATH`. Otherwise, uncomment the `set` directive and complete the path.

5. Import `"esp01.h"` and use the driver in your code (see the [test example](#test-example))

## Test example

TODO :)