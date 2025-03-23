#include "bsec_manager.h"
#include "config_manager.h"
#include "mqtt_manager.h"
#include "wifi_manager.h"
#include <Arduino.h>
#include <Wire.h>

MQTTManager mqttManager;
WifiManager wifiManager;
BSECManager bsecManager;
ConfigManager configManager;
long lastMsg = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    /* Valid for boards with USB-COM. Wait until the port is open */
    while (!Serial)
        delay(10);

    configManager.init(
        "home/weather/in",
        0x77,
        LED_BUILTIN,
        100);
    // Led adress here as well
    pinMode(configManager.getPanicLed(), OUTPUT);

    wifiManager.init(
        configManager.getWifiSsid(),
        configManager.getWifiPassword());

    mqttManager.init(
        configManager.getMqttServer(),
        configManager.getMqttPort(),
        configManager.getMqttUsername(),
        configManager.getMqttPassword(),
        configManager.getMqttTopic());

    bsecManager.init(
        configManager.getBsecAdress(),
        configManager.getPanicLed());

    bsecManager.setDataCallback(mqttManager.publishMessage);

    wifiManager.connect();

    mqttManager.setServer();
}

void loop()
{
    if (!wifiManager.isConnected()) {
        wifiManager.connect();
    }

    if (!mqttManager.isConnected()) {
        mqttManager.connect();
    }

    if (!bsecManager.isRunning()) {
        bsecManager.checkBsecStatus();
    }
}
