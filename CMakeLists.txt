cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Path to the Raspberry Pi Pico SDK")
set(PICO_SDK_INIT_CMAKE_FILE ${PICO_SDK_PATH}/pico_sdk_init.cmake)

set(MQTT_SERVER_IP CACHE STRING "IP to connect to")

include(${PICO_SDK_INIT_CMAKE_FILE})

project(picosense C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# defines the display target
add_subdirectory(lcd)

add_executable(picosense 
  picosense.c 
  sensor_reporter.c 
  sensor_reader.c 
  ${display_SRCS})

target_include_directories(picosense PRIVATE ${CMAKE_SOURCE_DIR} ${display_DIRS})

target_link_libraries(picosense PRIVATE
    pico_lwip_mqtt
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
    pico_malloc
    hardware_i2c
    hardware_spi
    hardware_pwm)

target_compile_definitions(picosense PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
    MQTT_SERVER_IP=\"${MQTT_SERVER_IP}\"
    PICO_SDK=1
    ST7565=1)

pico_enable_stdio_usb(picosense 1)
pico_enable_stdio_uart(picosense 0)

pico_add_extra_outputs(picosense)

