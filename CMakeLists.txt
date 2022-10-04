cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(picosense C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(picosense picosense.c)

target_link_libraries(picosense PRIVATE 
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib)

target_compile_definitions(picosense PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
    MQTT_SERVER_IP=\"${MQTT_SERVER_IP}\")

pico_enable_stdio_usb(picosense 1)
pico_enable_stdio_uart(picosense 0)

pico_add_extra_outputs(picosense)

