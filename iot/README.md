# NodeMCU 8266 IoT - Temperature and Humidity Program

The `NodeMCU IoT - Temperature and Humidity Program` is an embbeded program to  acquire temperature and humidity from a room. This project is a part of a final
project for my Industrial Automation Process Specialization at [Universidade Federal do Piauí](https://ufpi.br/).

## How to build and upload to nodemcu
Before following the steps below, change the variables for your WI-FI connection and MQTT Broker connection for the ones that will be used.

1. First connect your NodeMCU 8266 to your pc, and download any necessary driver that may be required;
2. After that you should have installed the [Platform.io CLI](https://platformio.org/);
3. Now in a terminal run the following command:
```
pio run --target upload
```

That is, now your project is running on your NodeMCU 8266.