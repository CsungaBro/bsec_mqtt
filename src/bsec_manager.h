#ifndef BSEC_MANAGER_H
#define BSEC_MANAGER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <bsec2.h>

class BSECManager
{
private:
    float _sampleRate;
    int _panicLed;
    int _errorDur;
    bsecSensor _sensorList[14];

    Bsec2 envSensor;

    void setTemperatureOffset();

    void subscribeSensors();

    void BSECManager::printBSECVerion();

    static void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

    void errLeds();

public:
    void init(int bsecAdress, int ledAdress);

    void checkBsecStatus();

    bool isRunning();
};

#endif