#include "wifi_manager.h"

WifiManager::WifiManager() : _ssid(nullptr),
                             _password(nullptr)
{
}

void WifiManager::init(const char *ssid, const char *password)
{
    _ssid = ssid;
    _password = password;
}

bool WifiManager::connect()
{
    if (_ssid == nullptr || _password == nullptr)
    {
        Serial.println("WiFi credentials are yet set");
        return false;
    }

    if (isConnected())
    {
        return true;
    }

    WiFi.disconnect();
    delay(100);

    WiFi.begin(_ssid, _password);

    unsigned long startTime = millis();
    while (!isConnected())
    {
        delay(500);
        Serial.print(".");

        if (millis() - startTime > 10000)
        {
            Serial.println("\nFailed to connect to the WiFi");
            return false;
        }
    }

    onConnected();

    return true;
}

void WifiManager::disconnect()
{
    WiFi.disconnect(true);
    Serial.println("WiFi disconnected");
}

IPAddress WifiManager::getLocalIP() const
{
    return WiFi.localIP();
}

bool WifiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

void WifiManager::onConnected() const
{
    // Event handler when connection is established
    Serial.println("WiFi connected event");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void WifiManager::printCred() const
{
    Serial.print("SSID: ");
    Serial.println(_ssid);
    Serial.print("Password: ");
    Serial.println(_password);
}