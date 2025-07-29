# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(pico_sdk_import.cmake)
project(main C CXX ASM)
pico_sdk_init()

add_subdirectory(libs/FatFs_SPI)

add_executable(${PROJECT_NAME} main.c libs/hw_config.c libs/buttons.c libs/buzzer.c libs/leds.c libs/display.c libs/imu.c libs/secure_digital_card.c)

pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Add the standard library to the build
target_link_libraries(
    ${PROJECT_NAME}
    pico_stdlib
    FatFs_SPI
    hardware_clocks
    hardware_i2c
    hardware_pwm
)

pico_add_extra_outputs(${PROJECT_NAME})