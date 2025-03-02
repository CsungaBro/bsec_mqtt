#include <Arduino.h>
#include <secrets.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <bsec2.h>

/* Macros used */
#define PANIC_LED LED_BUILTIN
#define ERROR_DUR 100
#define SAMPLE_RATE BSEC_SAMPLE_RATE_LP
// Macro to read build flags
#define ST(A) #A
#define STR(A) ST(A)

// Classes for grouping
/*WiFi class*/
class MyWiFi
{
public:
  const char *ssid;
  const char *password;
  void setup_wifi();
  WiFiClass &wifi;
  MyWiFi(WiFiClass &w) : wifi(w)
  {
#ifdef WIFI_SSID
    ssid = STR(WIFI_SSID);
#endif

#ifdef WIFI_PASSWORD
    password = STR(WIFI_PASSWORD);
#endif
  }
};

void MyWiFi::setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println(password);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*MQTT class*/
class MyMQTT
{
public:
  const char *server;
  int port;
  const char *username;
  const char *password;
  const char *topic;
  PubSubClient &client;
  void reconnect(PubSubClient &client);
  void publish(const char *payLoad);
  MyMQTT(PubSubClient &c) : client(c)
  {

#ifdef MQTT_SERVER
    server = STR(MQTT_SERVER);
#endif

#ifdef MQTT_PORT
    port = atoi(STR(MQTT_PORT));
#endif

#ifdef MQTT_USERNAME
    username = STR(MQTT_USERNAME);
#endif

#ifdef MQTT_PASSWORD
    password = STR(MQTT_PASSWORD);
#endif
    topic = "home/weather/in";
  }
};
void MyMQTT::reconnect(PubSubClient &client)
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId);
    Serial.println(username);
    Serial.println(password);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, password))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void MyMQTT::publish(const char *payLoad)
{
  if (client.connected())
  {
    client.publish(topic, payLoad);
  }
  else
  {
    Serial.print("MQTT not connected, not publishing data");
  }
}

// put function declarations here:

/* Helper functions declarations */
/**
 * @brief : This function toggles the led when a fault was detected
 */
void errLeds(void);

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);

/**
 * @brief : This function is called by the BSEC library when a new output is available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

void setup_bsec();

/* Create an object of the class Bsec2 */
Bsec2 envSensor;
WiFiClient espClient;
PubSubClient client(espClient);

MyMQTT mqtt(client);
MyWiFi wifi(WiFi);
long lastMsg = 0;

void setup()
{

  Serial.begin(115200);
  Wire.begin();
  pinMode(PANIC_LED, OUTPUT);

  /* Valid for boards with USB-COM. Wait until the port is open */
  while (!Serial)
    delay(10);

  setup_bsec();

  wifi.setup_wifi();

  mqtt.client.setServer(mqtt.server, mqtt.port);
}

void loop()
{
  if (!envSensor.run())
  {
    checkBsecStatus(envSensor);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    wifi.setup_wifi();
  }

  if (!mqtt.client.connected())
  {
    mqtt.reconnect(client);
  }
}

void errLeds(void)
{
  while (1)
  {
    digitalWrite(PANIC_LED, HIGH);
    delay(ERROR_DUR);
    digitalWrite(PANIC_LED, LOW);
    delay(ERROR_DUR);
  }
}

void setup_bsec()
{
  /* Desired subscription list of BSEC2 outputs */
  bsecSensor sensorList[] = {
      BSEC_OUTPUT_IAQ,
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
      BSEC_OUTPUT_COMPENSATED_GAS};

  /* Initialize the library and interfaces */
  if (!envSensor.begin(0x77, Wire))
  {
    checkBsecStatus(envSensor);
  }
  /*
   *	The default offset provided has been determined by testing the sensor in LP and ULP mode on application board 3.0
   *	Please update the offset value after testing this on your product
   */
  if (SAMPLE_RATE == BSEC_SAMPLE_RATE_ULP)
  {
    envSensor.setTemperatureOffset(TEMP_OFFSET_ULP);
  }
  else if (SAMPLE_RATE == BSEC_SAMPLE_RATE_LP)
  {
    envSensor.setTemperatureOffset(TEMP_OFFSET_LP);
  }

  /* Subsribe to the desired BSEC2 outputs */
  if (!envSensor.updateSubscription(sensorList, ARRAY_LEN(sensorList), SAMPLE_RATE))
  {
    checkBsecStatus(envSensor);
  }

  /* Whenever new data is available call the newDataCallback function */
  envSensor.attachCallback(newDataCallback);

  Serial.println("BSEC library version " +
                 String(envSensor.version.major) + "." + String(envSensor.version.minor) + "." + String(envSensor.version.major_bugfix) + "." + String(envSensor.version.minor_bugfix));
}

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec)
{
  StaticJsonDocument<160> doc;
  char output[160];
  if (!outputs.nOutputs)
  {
    return;
  }

  Serial.println("BSEC outputs:\n\tTime stamp = " + String((int)(outputs.output[0].time_stamp / INT64_C(1000000))));
  for (uint8_t i = 0; i < outputs.nOutputs; i++)
  {
    const bsecData output = outputs.output[i];
    switch (output.sensor_id)
    {
    case BSEC_OUTPUT_IAQ:
      Serial.println("\tIAQ = " + String(output.signal));
      Serial.println("\tIAQ accuracy = " + String((int)output.accuracy));
      doc["q"] = output.signal;
      doc["qa"] = output.accuracy;
      break;
    case BSEC_OUTPUT_RAW_TEMPERATURE:
      Serial.println("\tTemperature = " + String(output.signal));
      break;
    case BSEC_OUTPUT_RAW_PRESSURE:
      Serial.println("\tPressure = " + String(output.signal));
      doc["p"] = output.signal;
      break;
    case BSEC_OUTPUT_RAW_HUMIDITY:
      Serial.println("\tHumidity = " + String(output.signal));
      break;
    case BSEC_OUTPUT_RAW_GAS:
      Serial.println("\tGas resistance = " + String(output.signal));
      break;
    case BSEC_OUTPUT_STABILIZATION_STATUS:
      Serial.println("\tStabilization status = " + String(output.signal));
      doc["st"] = output.signal;
      break;
    case BSEC_OUTPUT_RUN_IN_STATUS:
      Serial.println("\tRun in status = " + String(output.signal));
      doc["ost"] = output.signal;
      break;
    case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
      Serial.println("\tCompensated temperature = " + String(output.signal));
      doc["t"] = output.signal;
      break;
    case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
      doc["h"] = output.signal;
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

  serializeJson(doc, output);
  Serial.println(output);
  mqtt.publish(output);
}

void checkBsecStatus(Bsec2 bsec)
{
  if (bsec.status < BSEC_OK)
  {
    Serial.println("BSEC error code : " + String(bsec.status));
    errLeds(); /* Halt in case of failure */
  }
  else if (bsec.status > BSEC_OK)
  {
    Serial.println("BSEC warning code : " + String(bsec.status));
  }

  if (bsec.sensor.status < BME68X_OK)
  {
    Serial.println("BME68X error code : " + String(bsec.sensor.status));
    errLeds(); /* Halt in case of failure */
  }
  else if (bsec.sensor.status > BME68X_OK)
  {
    Serial.println("BME68X warning code : " + String(bsec.sensor.status));
  }
}