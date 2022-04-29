package mqtt

import (
	"app/cmd/mqtt/handler"
	iaLogger "app/cmd/utils/logger"
	"fmt"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"os"
)

var logger = iaLogger.NewIaLogger()

func subscribeToIotSendDataTopic(client *mqtt.Client) {
	topic := "ind-assistant/sensors/send-data"
	token := (*client).Subscribe(topic, 1, handler.ReceiveIotData)
	token.Wait()
	logger.LogInfo(fmt.Sprintf("subscribed to topic: %s", topic))
}

func subscribeToIotLogTopic(client *mqtt.Client) {
	topic := "ind-assistant/sensors/log"
	token := (*client).Subscribe(topic, 1, handler.ReceiveIotLog)
	token.Wait()
	logger.LogInfo(fmt.Sprintf("subscribed to topic: %s", topic))
}

func NewMqttConn() {
	opts := mqtt.NewClientOptions()
	opts.AddBroker(os.Getenv("MQTT_BROKER"))
	opts.SetClientID("go_mqtt_client")
	opts.SetUsername("")
	opts.SetPassword("")
	opts.OnConnect = handler.Connect
	opts.OnConnectionLost = handler.ConnectionLost

	client := mqtt.NewClient(opts)
	token := client.Connect()
	if token.Wait() && token.Error() != nil {
		logger.LogError("failed to connect to mosquitto")
		logger.LogError(fmt.Sprintf("mqtt_broker %s", os.Getenv("MQTT_BROKER")))
	}
	logger.LogInfo("connected to mosquitto")

	subscribeToIotSendDataTopic(&client)
	subscribeToIotLogTopic(&client)
}
