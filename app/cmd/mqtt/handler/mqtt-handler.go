package handler

import (
	"fmt"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

var Connect mqtt.OnConnectHandler = func(client mqtt.Client) {
	logger.LogInfo("connected to mqtt")
}

var ConnectionLost mqtt.ConnectionLostHandler = func(client mqtt.Client, err error) {
	logger.LogInfo(fmt.Sprintf("connection lost: %v", err))
}
