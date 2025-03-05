#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <Preferences.h>

class ConfigManager
{
private:
    String _wifi_ssid;
    String _wifi_password;
    String _mqtt_server;
    int _mqtt_port;
    String _mqtt_username;
    String _mqtt_password;
    String _mqtt_topic;
    int _bsecAdress;
    int _panicLed;
    int _errorDur;

    WiFiClient wifiClient;
    PubSubClient mqttClient;
    Preferences preferences;

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