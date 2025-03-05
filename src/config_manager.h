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
    int _bsecAdress;
    int _panicLed;
    int _errorDur;

    WiFiClient wifiClient;
    PubSubClient mqttClient;

public:
    ConfigManager();
    void init(
        const char *mqtt_topic,
        int bsecAdress,
        int panicLed,
        int errorDur);

    const char *getWifiSsid() const;
    const char *getWifiPassword() const;
    const char *getMqttServer() const;
    int getMqttPort() const;
    const char *getMqttUsername() const;
    const char *getMqttPassword() const;
    const char *getMqttTopic() const;
    int getBsecAdress() const;
    int getPanicLed() const;
    int getErrorDur() const;
};

#endif