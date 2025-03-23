#ifndef BSEC_MANAGER_H
#define BSEC_MANAGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <bsec2.h>

using SensorDataCallback = std::function<void(const JsonDocument&)>;

class BSECManager {
private:
    float _sampleRate;
    int _panicLed;
    int _errorDur;
    bsecSensor _sensorList[14];
    Bsec2 envSensor;

    StaticJsonDocument<256> _lastReadings;
    bool _newDataAvailable;
    unsigned long _lastReadingTime;

    SensorDataCallback _dataCallback;

    void setTemperatureOffset();
    void subscribeSensors();
    void printBSECVerion();
    void errLeds();

    static void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

    static BSECManager _instance;

public:
    BSECManager();

    void init(int bsecAdress, int ledAdress);
    void checkBsecStatus();
    bool isRunning();

    bool hasNewData() const;
    unsigned long getLastReadingTime() const;
    String getLastReading();

    void setDataCallback(SensorDataCallback callback);

    void processData(const bme68xData data, const bsecOutputs& outputs);
};

#endif