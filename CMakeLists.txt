cmake_minimum_required(VERSION 3.21)
project(F1_MQTT)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp include/MQTT_Handler.h src/MQTT_Handler.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${PROJECT_NAME} lib/paho_c/include include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/paho_c/paho-mqtt3c.lib)

