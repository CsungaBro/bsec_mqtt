#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#include <Arduino.h>
#include <WiFi.h>

class WifiManager
{
private:
    const char *_ssid;
    const char *_password;

public:
    WifiManager();

    // Initializtaion of the class
    void init(const char *ssid, const char *password);

    // Connect to the wifi network
    bool connect();

    // Disconect from the wifi network
    void disconnect();

    // Getter for connection status
    bool isConnected() const;

    // Getter for the IP Adress
    IPAddress getLocalIP() const;

    // Print infos while connected
    void onConnected() const;

    void printCred() const;
};
#endif