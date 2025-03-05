#include "config_manager.h"
#include <Preferences.h>

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
    const char *mqtt_topic,
    int bsecAdress,
    int panicLed,
    int errorDur)
{

    Preferences preferences;
    preferences.begin("ssid", true);
    _wifi_ssid = preferences.getString("ssid").c_str();
    _wifi_password = preferences.getString("password").c_str();
    preferences.end();

    preferences.begin("mqtt", true);
    _mqtt_server = preferences.getString("server").c_str();
    _mqtt_port = preferences.getInt("port");
    _mqtt_username = preferences.getString("username").c_str();
    _mqtt_password = preferences.getString("password").c_str();
    _mqtt_topic = mqtt_topic;
    _bsecAdress = bsecAdress;
    _panicLed = panicLed;
    _errorDur = errorDur;
    preferences.end();
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