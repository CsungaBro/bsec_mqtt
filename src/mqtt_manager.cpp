#include "mqtt_manager.h"

MQTTManager::MQTTManager()
    : mqttClient(wifiClient)
    , _server(nullptr)
    , _port(-1)
    , _username(nullptr)
    , _password(nullptr)
    , _topic(nullptr)

{
}

void MQTTManager::init(
    const char* server,
    int port,
    const char* username,
    const char* password,
    const char* topic)
{
    _server = server;
    _port = port;
    _username = username;
    _password = password;
    _topic = topic;
}

bool MQTTManager::setServer()
{
    if (_server == nullptr) {
        Serial.println("MQTT Server not defined");
        return false;
    }
    if (_port == -1) {
        Serial.println("MQTT Port not defined");
        return false;
    }

    mqttClient.setServer(_server, _port);

    Serial.println("MQTT Server set");

    return true;
}

bool MQTTManager::connect()
{
    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);
        Serial.println(_username);
        Serial.println(_password);

        // Attempt to connect
        if (mqttClient.connect(clientId.c_str(), _username, _password)) {
            Serial.println("connected");
            return true;
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            delay(5000);
        }
    }
    return false;
}

void MQTTManager::publishMessage(const JsonDocument& doc)
{
    char payload[256];
    serializeJson(doc, payload, sizeof(payload));

    if (mqttClient.connected()) {
        int res = mqttClient.publish(_topic, payload);
        if (res) {
            Serial.println("MQTT data sent");
        } else {
            Serial.println("Something went wrong");
        }
    } else {
        Serial.print("MQTT not connected, not publishing data");
    }
};

bool MQTTManager::isConnected()
{
    return mqttClient.connected();
}