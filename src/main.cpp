#include <Arduino.h>
#include <Wire.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "bsec_manager.h"
#include "config_manager.h"

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

  configManager.init("home/weather/in", 0x77, LED_BUILTIN, 100);
  // Led adress here as well
  pinMode(configManager.getPanicLed(), OUTPUT);

  Serial.println();
  Serial.println("Credentials from the main configmanagar");
  Serial.println(configManager.getWifiSsid());
  Serial.println(configManager.getWifiPassword());
  Serial.println();

  const char *aa = configManager.getWifiSsid();
  wifiManager.init(
      aa,
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

  wifiManager.connect();

  mqttManager.setServer();
}

void loop()
{
  if (!bsecManager.isRunning())
  {
    bsecManager.checkBsecStatus();
  }

  if (!wifiManager.isConnected())
  {
    wifiManager.connect();
  }

  if (!mqttManager.isConnected())
  {
    mqttManager.connect();
  }
}
