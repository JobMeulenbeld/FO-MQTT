//
// Created by meule on 5/4/2023.
//
#include "../include/MQTT_Handler.h"

void MQTT_Handler::connlost(void *context, char *cause) {
    auto* handler = (MQTT_Handler*)context;
    handler->_connected = false;
}

int msg(void *context, char *topicName, int topicLen, MQTTClient_message *message){
    return 0;
}

void refresh(MQTT_Handler* handler) {
    while(!handler->quit()){
        if(!handler->state() && !handler->busy()){
            if(handler->connect() == 0){
                //Add subscription here

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

MQTT_Handler::MQTT_Handler(const std::string& host, const std::string& client_id, int (*callback)(void *context, char *topicName, int topicLen, MQTTClient_message *message)) {

    if(callback){
        _msg_callback = callback;
    }
    else{
        _msg_callback = &msg;
    }

    int rc = MQTTClient_create(&_client, host.c_str(), client_id.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if(rc != MQTTCLIENT_SUCCESS){
        std::cerr << "MQTT: " << "Failed to create a _client -> " << "ERR CODE: " << rc << std::endl;
        exit(EXIT_FAILURE);
    }

    rc = MQTTClient_setCallbacks(_client, this, connlost, _msg_callback, NULL)  ;
    if ( rc != MQTTCLIENT_SUCCESS){
        std::cerr << "MQTT: " << "Failed to set callbacks -> " << "ERR CODE: " << rc << std::endl;
        exit(EXIT_FAILURE);
    }

    _conn_opts = MQTTClient_connectOptions_initializer;
    _conn_opts.connectTimeout = 5;
    _conn_opts.keepAliveInterval = 20;
    _conn_opts.cleansession = 1;

    _refresh = std::thread(refresh, this);
}

MQTT_Handler::~MQTT_Handler() {
    _quit = true;
    _refresh.join();
    MQTTClient_disconnect(_client, 200);
    MQTTClient_destroy(&_client);
}

int MQTT_Handler::publish(const std::string& topic, const char *data, int len) {
    if(!_connected){
        std::cerr << "Can't publish, no connection" << std::endl;
        return -1;
    }
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void*)data;
    pubmsg.payloadlen = len;
    pubmsg.qos = 0;
    pubmsg.retained = 0;

    int rc = MQTTClient_publishMessage(_client, topic.c_str(), &pubmsg, NULL);
    if (rc != MQTTCLIENT_SUCCESS){
        std::cerr << "MQTT: " << "Failed to publish message -> " << "ERR CODE: " << rc << std::endl;
        return -1;
    }

    return 0;
}

int MQTT_Handler::subscribe(const std::string& topic, int qos) {
    if(!_connected){
        std::cerr << "Can't subscribe, no connection" << std::endl;
        return -1;
    }

    int rc = MQTTClient_subscribe(_client, topic.c_str(), qos);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        std::cerr << "MQTT: " << "Failed to subscribe -> " << "ERR CODE: " << rc << std::endl;
        return -1;
    }

    return 0;
}

int MQTT_Handler::connect() {
    _busy = true;
    switch (MQTTClient_connect(_client, &_conn_opts)) {
        case -1:{
            std::cerr << "MQTT: " << "Failed to connect" << std::endl;
            _busy = false;
            return -1;
        }
        case 0:{
            std::cout << "MQTT: " << "Connected successfully" << std::endl;
            _connected = true;
            _busy = false;
            return 0;
        }
        case 1:{
            std::cerr << "MQTT: " << "Connection refused: Unacceptable protocol version" << std::endl;
            _busy = false;
            return 1;
        }
        case 2:{
            std::cerr << "MQTT: " << "Connection refused: Identifier rejected" << std::endl;
            _busy = false;
            return 2;
        }
        case 3:{
            std::cerr << "MQTT: " << "Connection refused: Server unavailable" << std::endl;
            _busy = false;
            return 3;
        }
        case 4:{
            std::cerr << "MQTT: " << "Connection refused: Bad user name or password" << std::endl;
            _busy = false;
            return 4;
        }
        case 5:{
            std::cerr << "MQTT: " << "Connection refused: Not authorized" << std::endl;
            _busy = false;
            return 5;
        }
    }
    return -1;
}




