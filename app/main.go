package main

import (
	mqttClient "app/cmd/mqtt"
	"github.com/gin-gonic/gin"
	"github.com/joho/godotenv"
	"log"
)

func main() {
	envErr := godotenv.Load()
	if envErr != nil {
		log.Fatal("Error loading .env file")
	}
	mqttClient.NewMqttConn()

	server := gin.Default()
	//router.InitRoutes(server)

	err := server.Run("localhost:8080")
	if err != nil {
		panic(err)
	}
}
