cmake_minimum_required(VERSION 3.21)

include_directories(${PROJECT_NAME} lib/paho_c/include)
add_library(${PROJECT_NAME} STATIC src/MQTT_Handler.cpp include/MQTT_Handler.h)
configure_file(lib/paho_c/paho-mqtt3c.dll ${CMAKE_BINARY_DIR} COPYONLY)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/paho_c/paho-mqtt3c.dll)

