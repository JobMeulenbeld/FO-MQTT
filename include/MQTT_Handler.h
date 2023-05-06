//
// Created by meule on 5/4/2023.
//

#ifndef F1_MQTT_MQTT_HANDLER_H
#define F1_MQTT_MQTT_HANDLER_H

#include <iostream>
#include <chrono>
#include <thread>

#include <string>
#include <iostream>
#include <MQTTClient.h>

class MQTT_Handler {
public:
    MQTT_Handler(const std::string& host, const std::string& client_id, int (*callback)(void *context, char *topicName, int topicLen, MQTTClient_message *message) = NULL);
    ~MQTT_Handler();

    int connect();
    int publish(const std::string& topic, const char* data, int len);
    int subscribe(const std::string& topic, int qos = 0);

    bool state(){return _connected;}
    bool busy(){return _busy;}
    bool quit(){return _quit;}

    static void connlost(void *context, char *cause);

private:

    MQTTClient _client;
    MQTTClient_connectOptions _conn_opts;

    bool _connected;
    bool _busy = false;
    bool _quit = false;

    int (*_msg_callback)(void *context, char *topicName, int topicLen, MQTTClient_message *message);

    std::thread _refresh;

};


#endif //F1_MQTT_MQTT_HANDLER_H


