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

#ifndef IOT_IALOGGER_H
#define IOT_IALOGGER_H

#include "PubSubClient.h"
#include <ArduinoJson.h>

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
};

class IaLogger {
public:

    IaLogger();

    ~IaLogger() = default;

    void init(const char *topic, PubSubClient *mqttClient, const char *sensorId);

    void log(const char *msg, LogLevel logLevel);

    void setShouldSerialPrint(bool shouldSerialPrint);

private:
    const char *_topic;
    PubSubClient *_client;
    const char *_sensorId;
    bool _shouldSerialPrint = true;


};

#endif //IOT_IALOGGER_H
