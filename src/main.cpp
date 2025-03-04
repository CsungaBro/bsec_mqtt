#include <Arduino.h>
#include <Wire.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "bsec_manager.h"

MQTTManager mqttManager;
WifiManager wifiManager;
BSECManager bsecManager;
long lastMsg = 0;

void setup()
{
  mqttManager.init("", -1, "", "", "");
  wifiManager.init("", "");
  bsecManager.init(0, 0);

  Serial.begin(115200);
  Wire.begin();
  // Led adress here as well
  pinMode(0, OUTPUT);

  /* Valid for boards with USB-COM. Wait until the port is open */
  while (!Serial)
    delay(10);

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
