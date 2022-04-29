package model

type GenericData struct {
	Value float64 `json:"value"`
	Scale string  `json:"scale"`
}

type IotSensorData struct {
	SensorID    string      `json:"sensor_id"`
	Humidity    GenericData `json:"humidity"`
	Temperature GenericData `json:"temperature"`
}

type IotLogData struct {
	SensorID   string `json:"sensor_id"`
	Timestamp  string `json:"timestamp"`
	LogLevel   string `json:"log_level"`
	LogMessage string `json:"log_message"`
}
