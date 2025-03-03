#include "config_manager.h"

ConfigManager::ConfigManager() : _wifi_ssid(nullptr),
                                 _wifi_password(nullptr),
                                 _mqtt_server(nullptr),
                                 _mqtt_port(0),
                                 _mqtt_username(nullptr),
                                 _mqtt_password(nullptr),
                                 _mqtt_topic(nullptr)

{
}

void ConfigManager::init(
    const char *wifi_ssid,
    const char *wifi_password,
    const char *mqtt_server,
    int mqtt_port,
    const char *username,
    const char *mqtt_password,
    const char *mqtt_topic)
{
    _wifi_ssid = wifi_ssid;
    _wifi_password = wifi_password;
    _mqtt_server = mqtt_server;
    _mqtt_port = mqtt_port;
    _mqtt_username = username;
    _mqtt_password = mqtt_password;
    _mqtt_topic = mqtt_topic;
};