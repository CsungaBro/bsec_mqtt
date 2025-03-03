#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class ConfigManager
{
private:
    const char *_wifi_ssid;
    const char *_wifi_password;
    const char *_mqtt_server;
    int _mqtt_port;
    const char *_mqtt_username;
    const char *_mqtt_password;
    const char *_mqtt_topic;

    WiFiClient wifiClient;
    PubSubClient mqttClient;

public:
    void init(
        const char *wifi_ssid,
        const char *wifi_password,
        const char *mqtt_server,
        int mqtt_port,
        const char *mqtt_username,
        const char *mqtt_password,
        const char *mqtt_topic);
};

#endif