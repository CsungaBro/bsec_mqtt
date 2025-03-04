#include "config_manager.h"

ConfigManager::ConfigManager() : _wifi_ssid(nullptr),
                                 _wifi_password(nullptr),
                                 _mqtt_server(nullptr),
                                 _mqtt_port(0),
                                 _mqtt_username(nullptr),
                                 _mqtt_password(nullptr),
                                 _mqtt_topic(nullptr),
                                 _bsecAdress(0),
                                 _panicLed(0),
                                 _errorDur(0)

{
}

void ConfigManager::init(
    const char *wifi_ssid,
    const char *wifi_password,
    const char *mqtt_server,
    int mqtt_port,
    const char *username,
    const char *mqtt_password,
    const char *mqtt_topic,
    int bsecAdress,
    int panicLed,
    int errorDur)
{
    _wifi_ssid = wifi_ssid;
    _wifi_password = wifi_password;
    _mqtt_server = mqtt_server;
    _mqtt_port = mqtt_port;
    _mqtt_username = username;
    _mqtt_password = mqtt_password;
    _mqtt_topic = mqtt_topic;
    _bsecAdress = bsecAdress;
    _panicLed = panicLed;
    _errorDur = errorDur;
};

const char *ConfigManager::getWifiSsid() const { return _wifi_ssid; }
const char *ConfigManager::getWifiPassword() const { return _wifi_password; }
const char *ConfigManager::getMqttServer() const { return _mqtt_server; }
int ConfigManager::getMqttPort() const { return _mqtt_port; }
const char *ConfigManager::getMqttUsername() const { return _mqtt_username; }
const char *ConfigManager::getMqttPassword() const { return _mqtt_password; }
const char *ConfigManager::getMqttTopic() const { return _mqtt_topic; }
int ConfigManager::getBsecAdress() const { return _bsecAdress; }
int ConfigManager::getPanicLed() const { return _panicLed; }
int ConfigManager::getErrorDur() const { return _errorDur; }