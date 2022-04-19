// MIT License
//
// Copyright (c) 2022 Augusto Alves Silva
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <IaLogger.h>


#define SUBSCRIBE_TOPIC "topic/to/subscribe"
#define SEND_DATA_TOPIC "topic/to/send-data"
#define SEND_LOG_TOPIC "topic/to/send-log"
#define SENSOR_ID "UUID"


// mapping available pins from NODEMCU
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

// Delay
const unsigned long delayDuration = 59500; // 59,5 s

// DHT
DHT dht(D6, DHT22);

// WIFI
const char *SSID = "WIFI_SSID";
const char *PASSWORD = "WIFI_PASSWORD";


// MQTT
const char *BROKER_MQTT = "BROKET_MQTT";
int BROKER_PORT = 1883;

// WiFi
WiFiClient espClient;
PubSubClient MQTT(espClient);

// Logger
IaLogger logger;

void reconectWiFi()
{

    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(D8, LOW);
        delay(100);
        Serial.print(".");
        digitalWrite(D8, HIGH);
        delay(100);
    }

    digitalWrite(D8, HIGH);
    String msgToLog;
    msgToLog = "Successfully connected to the WIFI network: " + String(SSID);
    logger.log(msgToLog.c_str(), LogLevel::INFO);
    msgToLog = "IP adressed: " + WiFi.localIP().toString();
    logger.log(msgToLog.c_str(), LogLevel::INFO);
}

void initWiFi()
{
    delay(10);
    String msgToLog;
    msgToLog = "Connecting to WIFI network: " + String(SSID);
    logger.log(msgToLog.c_str(), LogLevel::INFO);
    logger.log("Awaiting the connection", LogLevel::INFO);

    reconectWiFi();
}

void mqttCallback(char *topic, const byte *payload, unsigned int length)
{
    String parsedPayload;
    String msgToLog;

    // gets the payload as a string
    for (unsigned int i = 0; i < length; i++)
    {
        char c = (char)payload[i];
        parsedPayload += c;
    }

    StaticJsonDocument<256> json;

    DeserializationError error = deserializeJson(json, parsedPayload.c_str());
    if (error.code() != ArduinoJson6192_F1::DeserializationError::Ok) {
        msgToLog = "An error happened while trying to deserialize the command json: " + parsedPayload;
        logger.log(msgToLog.c_str(), LogLevel::ERROR);
    }

    // Emulates a command to turn OFF or ON a FAN based on a temperature
    // or turn OFF or ON a humidifier based on humidity
    JsonObject fan = json["fan"];
    const char* fan_cmd = fan["cmd"];
    JsonObject humidifier = json["humidifier"];
    const char* humidifier_cmd = humidifier["cmd"];

    if (strcmp(fan_cmd, "OFF") == 0) {
        digitalWrite(D4, LOW);
    }

    if (strcmp(fan_cmd, "ON") == 0) {
        digitalWrite(D4, HIGH);
    }

    if (strcmp(humidifier_cmd, "OFF") == 0) {
        digitalWrite(D3, LOW);
    }

    if (strcmp(humidifier_cmd, "ON") == 0) {
        digitalWrite(D3, HIGH);
    }
}

void initMQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqttCallback);
}

void reconnectMQTT()
{
    while (!MQTT.connected())
    {
        logger.log("Trying to connect to MQTT broker...", LogLevel::INFO);
        if (MQTT.connect(SENSOR_ID))
        {
            logger.log("Successfully connected to MQTT broker!", LogLevel::INFO);
            MQTT.subscribe(SUBSCRIBE_TOPIC);
        }
        else
        {
            logger.log("Fail to connect to MQTT broker, it will retry in 2s.", LogLevel::ERROR);
            delay(2000);
        }
    }
}

void checkWifiAndMqttConnections(void)
{
    if (!MQTT.connected())
        reconnectMQTT();

    reconectWiFi();
}

void sendDataToTopic(void)
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    StaticJsonDocument<256> json;
    char payload[256];

    json["sensor_id"] = SENSOR_ID;
    JsonObject humidity = json.createNestedObject("humidity");
    JsonObject temperature = json.createNestedObject("temperature");
    humidity["value"] = h;
    humidity["scale"] =  "relative";
    temperature["value"] = t;
    temperature["scale"] = "C";

    serializeJson(json, payload);

    Serial.println(payload);

    MQTT.publish(SEND_DATA_TOPIC, payload);

    digitalWrite(D7, HIGH);
    logger.log("Published new data to topic", LogLevel::INFO);
    delay(500);
    digitalWrite(D7, LOW);
    delay(delayDuration);
}

void setup()
{
    Serial.begin(115200);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    
    initWiFi();
    initMQTT();
    dht.begin();
    logger.init(SEND_LOG_TOPIC, &MQTT, SENSOR_ID);
}

void loop()
{

    checkWifiAndMqttConnections();

    sendDataToTopic();

    // MQTT broker's conn keep-alive
    MQTT.loop();
}