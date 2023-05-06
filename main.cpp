#include <cstring>
#include "MQTT_Handler.h"

int main(int argc, char* argv[])
{
    MQTT_Handler handler("tcp://192.168.68.107:1883", "FO-MQTT");

    while(true){
        handler.publish("Topic/World", "World", 5);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}