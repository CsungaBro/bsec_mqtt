#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class MQTTManager
{
private:
    const char *_server;
    int _port;
    const char *_username;
    const char *_password;
    const char *_topic;

    WiFiClient wifiClient;
    PubSubClient mqttClient;

public:
    void init(
        const char *server,
        int port,
        const char *username,
        const char *password,
        const char *topic);

    bool connect();

    bool setServer();

    void publishMessage(const char *payload);
};

#endif