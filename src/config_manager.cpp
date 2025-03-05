#include "config_manager.h"

ConfigManager::ConfigManager() : _mqtt_port(0),
                                 _bsecAdress(0),
                                 _panicLed(0),
                                 _errorDur(0)
{
}

void ConfigManager::init(
    const char *mqtt_topic,
    int bsecAdress,
    int panicLed,
    int errorDur)
{
    preferences.begin("wifi", true);
    _wifi_ssid = preferences.getString("ssid");
    _wifi_password = preferences.getString("password");
    preferences.end();

    preferences.begin("mqtt", true);
    _mqtt_server = preferences.getString("server");
    _mqtt_port = preferences.getInt("port");
    _mqtt_username = preferences.getString("username");
    _mqtt_password = preferences.getString("password");
    preferences.end();

    _mqtt_topic = mqtt_topic;
    _bsecAdress = bsecAdress;
    _panicLed = panicLed;
    _errorDur = errorDur;
}

const char *ConfigManager::getWifiSsid() const { return _wifi_ssid.c_str(); }
const char *ConfigManager::getWifiPassword() const { return _wifi_password.c_str(); }
const char *ConfigManager::getMqttServer() const { return _mqtt_server.c_str(); }
int ConfigManager::getMqttPort() const { return _mqtt_port; }
const char *ConfigManager::getMqttUsername() const { return _mqtt_username.c_str(); }
const char *ConfigManager::getMqttPassword() const { return _mqtt_password.c_str(); }
const char *ConfigManager::getMqttTopic() const { return _mqtt_topic.c_str(); }
int ConfigManager::getBsecAdress() const { return _bsecAdress; }
int ConfigManager::getPanicLed() const { return _panicLed; }
int ConfigManager::getErrorDur() const { return _errorDur; }