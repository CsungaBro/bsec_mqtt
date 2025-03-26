#include "bsec_manager.h"

BSECManager::BSECManager()
    : _sampleRate(BSEC_SAMPLE_RATE_LP)
    , _errorDur(100)
    , _panicLed(0)
    , _newDataAvailable(false)
    , _dataCallbackSet(false)
    , _lastReadingTime(0)
    , _sensorList { BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_STABILIZATION_STATUS,
        BSEC_OUTPUT_RUN_IN_STATUS,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_GAS_PERCENTAGE,
        BSEC_OUTPUT_COMPENSATED_GAS }
{
}

void BSECManager::init(int bsecAdress, int ledAdress)
{
    _panicLed = ledAdress;
    /* Initialize the library and interfaces */
    if (!envSensor.begin(bsecAdress, Wire)) {
        checkBsecStatus();
    }

    setTemperatureOffset();

    subscribeSensors();

    /* Whenever new data is available call the newDataCallback function */
    envSensor.attachCallback(newDataCallback);

    printBSECVerion();
};

void BSECManager::printBSECVerion()
{
    Serial.println("BSEC library version " + String(envSensor.version.major) + "." + String(envSensor.version.minor) + "." + String(envSensor.version.major_bugfix) + "." + String(envSensor.version.minor_bugfix));
}

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void BSECManager::checkBsecStatus()
{
    if (envSensor.status < BSEC_OK) {
        Serial.println("BSEC error code : " + String(envSensor.status));
        errLeds(); /* Halt in case of failure */
    } else if (envSensor.status > BSEC_OK) {
        Serial.println("BSEC warning code : " + String(envSensor.status));
    }

    if (envSensor.sensor.status < BME68X_OK) {
        Serial.println("BME68X error code : " + String(envSensor.sensor.status));
        errLeds(); /* Halt in case of failure */
    } else if (envSensor.sensor.status > BME68X_OK) {
        Serial.println("BME68X warning code : " + String(envSensor.sensor.status));
    };
}

void BSECManager::setTemperatureOffset()
{
    /*
     *	The default offset provided has been determined by testing the sensor in LP and ULP mode on application board 3.0
     *	Please update the offset value after testing this on your product
     */
    if (_sampleRate == BSEC_SAMPLE_RATE_ULP) {
        envSensor.setTemperatureOffset(TEMP_OFFSET_ULP);
    } else if (_sampleRate == BSEC_SAMPLE_RATE_LP) {
        envSensor.setTemperatureOffset(TEMP_OFFSET_LP);
    }
};

void BSECManager::subscribeSensors()
{
    /* Subsribe to the desired BSEC2 outputs */
    if (!envSensor.updateSubscription(
            _sensorList,
            ARRAY_LEN(_sensorList),
            _sampleRate)) {
        checkBsecStatus();
    }
};

/**
 * @brief : This function is called by the BSEC library when a new output is available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void BSECManager::newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec)
{
    this->processData(data, outputs);
};

/**
 * @brief : This function toggles the led when a fault was detected
 */
void BSECManager::errLeds(void)
{
    while (1) {
        digitalWrite(_panicLed, HIGH);
        delay(_errorDur);
        digitalWrite(_panicLed, LOW);
        delay(_errorDur);
    }
}

bool BSECManager::isRunning()
{
    return envSensor.run();
}

bool BSECManager::hasNewData() const
{
    return _newDataAvailable;
};
unsigned long BSECManager::getLastReadingTime() const
{
    return _lastReadingTime;
};
String BSECManager::getLastReading()
{
    String jsonStr;
    serializeJson(_lastReadings, jsonStr);
    return jsonStr;
}

void BSECManager::setDataCallback(SensorDataCallback callback)
{
    _dataCallback = callback;
    _dataCallbackSet = true;
}

void BSECManager::processData(const bme68xData data, const bsecOutputs& outputs)
{
    if (!outputs.nOutputs) {
        return;
    }

    _lastReadings.clear();

    Serial.println("BSEC outputs:\n\tTime stamp = " + String((int)(outputs.output[0].time_stamp / INT64_C(1000000))));
    for (uint8_t i = 0; i < outputs.nOutputs; i++) {
        const bsecData output = outputs.output[i];
        switch (output.sensor_id) {
        case BSEC_OUTPUT_IAQ:
            Serial.println("\tIAQ = " + String(output.signal));
            Serial.println("\tIAQ accuracy = " + String((int)output.accuracy));
            _lastReadings["q"] = output.signal;
            _lastReadings["qa"] = output.accuracy;
            break;
        case BSEC_OUTPUT_RAW_TEMPERATURE:
            Serial.println("\tTemperature = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_PRESSURE:
            Serial.println("\tPressure = " + String(output.signal));
            _lastReadings["p"] = output.signal;
            break;
        case BSEC_OUTPUT_RAW_HUMIDITY:
            Serial.println("\tHumidity = " + String(output.signal));
            break;
        case BSEC_OUTPUT_RAW_GAS:
            Serial.println("\tGas resistance = " + String(output.signal));
            break;
        case BSEC_OUTPUT_STABILIZATION_STATUS:
            Serial.println("\tStabilization status = " + String(output.signal));
            _lastReadings["st"] = output.signal;
            break;
        case BSEC_OUTPUT_RUN_IN_STATUS:
            Serial.println("\tRun in status = " + String(output.signal));
            _lastReadings["ost"] = output.signal;
            break;
        case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
            Serial.println("\tCompensated temperature = " + String(output.signal));
            _lastReadings["t"] = output.signal;
            break;
        case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
            _lastReadings["h"] = output.signal;
            Serial.println("\tCompensated humidity = " + String(output.signal));
            break;
        case BSEC_OUTPUT_STATIC_IAQ:
            Serial.println("\tStatic IAQ = " + String(output.signal));
            break;
        case BSEC_OUTPUT_CO2_EQUIVALENT:
            Serial.println("\tCO2 Equivalent = " + String(output.signal));
            break;
        case BSEC_OUTPUT_BREATH_VOC_EQUIVALENT:
            Serial.println("\tbVOC equivalent = " + String(output.signal));
            break;
        case BSEC_OUTPUT_GAS_PERCENTAGE:
            Serial.println("\tGas percentage = " + String(output.signal));
            break;
        case BSEC_OUTPUT_COMPENSATED_GAS:
            Serial.println("\tCompensated gas = " + String(output.signal));
            break;
        default:
            break;
        }
    }

    _newDataAvailable = true;
    _lastReadingTime = millis();

    Serial.println("Before the callbacks");
    Serial.println("Datacallback set: ");
    Serial.println(_dataCallbackSet);
    if (_dataCallback) {
        _dataCallback(_lastReadings);
    }
};