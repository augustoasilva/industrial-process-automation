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

#include "IaLogger.h"

void IaLogger::log(const char *msg, LogLevel logLevel) {
    String logMsg;
    String level;

    switch (logLevel) {
        case LogLevel::DEBUG:
            level = "DEBUG";
            logMsg = "[DEBUG] " + String(msg);
            break;
        case LogLevel::INFO:
            level = "INFO";
            logMsg = "[INFO] " + String(msg);
            break;
        case LogLevel::WARNING:
            level = "WARNING";
            logMsg = "[WARNING] " + String(msg);
            break;
        case LogLevel::ERROR:
            level = "ERROR";
            logMsg = "[ERROR] " + String(msg);
            break;
        default:
            level = "INFO";
            logMsg = "[INFO] " + String(msg);
            break;
    }

    StaticJsonDocument<256> json;
    char payload[256];
    json["sensor_id"] = _sensorId;
    json["log_level"] = level;
    json["log_message"] = msg;
    serializeJson(json, payload);

    if (_shouldSerialPrint) {
        Serial.println(logMsg);
    }
    if (_client != nullptr) {
        _client->publish(_topic, payload);
    }
}

void IaLogger::setShouldSerialPrint(bool shouldSerialPrint) {
    _shouldSerialPrint = shouldSerialPrint;
}

IaLogger::IaLogger() {

}

void IaLogger::init(const char *topic, PubSubClient *mqttClient, const char *sensorId) {
    _topic = topic;
    _client = mqttClient;
    _sensorId = sensorId;
}
