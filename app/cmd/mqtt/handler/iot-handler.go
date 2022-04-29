package handler

import (
	"app/cmd/mqtt/model"
	iaLogger "app/cmd/utils/logger"
	"encoding/json"
	"fmt"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"os"
	"time"
)

var logger = iaLogger.NewIaLogger()
var influxClient = influxdb2.NewClient(os.Getenv("INFLUX_URL"), os.Getenv("INFLUX_AUTH_TOKEN"))
var dataApi = influxClient.WriteAPI("tcc", "iot-temp-hum")
var dataApiErrorsCh = dataApi.Errors()
var logApi = influxClient.WriteAPI("tcc", "iot-sensors-log")
var logApiErrorsCh = dataApi.Errors()

var ReceiveIotData mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
	var data model.IotSensorData
	err := json.Unmarshal(msg.Payload(), &data)
	if err != nil {
		logger.LogError(fmt.Sprintf("failed to bind received raw message: %s \n because of %s", msg.Payload(), err.Error()))
	}

	go func() {
		for err := range dataApiErrorsCh {
			logger.LogError(fmt.Sprintf("failed to write a point to influx db because of error: %s", err.Error()))
		}
	}()

	logger.LogInfo(fmt.Sprintf("data %#v will be sent to influxdb to iot-temp-hum bucket", data))

	pTime := time.Now()
	p1 := influxdb2.NewPointWithMeasurement("stat").
		AddTag("sensor_id", data.SensorID).
		AddTag("type", "temperature").
		AddField("value", data.Temperature.Value).
		AddField("scale", data.Temperature.Scale).
		SetTime(pTime)
	p2 := influxdb2.NewPointWithMeasurement("stat").
		AddTag("sensor_id", data.SensorID).
		AddTag("type", "humidity").
		AddField("value", data.Humidity.Value).
		AddField("scale", data.Humidity.Scale).
		SetTime(pTime)
	dataApi.WritePoint(p1)
	dataApi.WritePoint(p2)
}

var ReceiveIotLog mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
	var logData model.IotLogData
	err := json.Unmarshal(msg.Payload(), &logData)
	logData.Timestamp = time.Now().UTC().Format(time.RFC3339)
	if err != nil {
		logger.LogError(fmt.Sprintf("failed to bind received raw message: %s \n because of %s", msg.Payload(), err.Error()))
	}

	go func() {
		for err := range logApiErrorsCh {
			logger.LogError(fmt.Sprintf("failed to write a point to influx db because of error: %s", err.Error()))
		}
	}()

	logger.LogInfo(fmt.Sprintf("log %#v will be sent to influxdb to iot-sensors-log bucket", logData))

	pTime := time.Now()
	p1 := influxdb2.NewPointWithMeasurement("stat").
		AddTag("sensor_id", logData.SensorID).
		AddTag("log_level", logData.LogLevel).
		AddTag("timestamp", logData.Timestamp).
		AddField("message", logData.LogMessage).
		SetTime(pTime)
	logApi.WritePoint(p1)
}
