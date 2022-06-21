#include "SCPI_handler.h"
#include "ScanerCore.h"
#include "Command.h"
#include "Printer.h"
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>


const int baudrate = 9600;

char ssid[] = "nyquist";
char pass[] = "314159pi";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
ScanerCore core(mqttClient);

const char broker[] = "192.168.2.47";
int        port     = 1883;
const char commandTopic[]  = "arduino/commands";

void setup() {
  Serial.begin(baudrate);
  core.init();
  
  core.SetPrinter(serial);
  int retry = 0;
  while (WiFi.begin(ssid, pass) != WL_CONNECTED or retry == 5) {
    delay(1000);
    retry++;
  }
  if (retry == 5) {
    core.Println("Wifi connection is not disponible");
  }

  if (!mqttClient.connect(broker, port)) {
    core.Println("Mqtt connection is not disponible");
  }
  else {
    mqttClient.subscribe(commandTopic);
  }
  core.Println("Instrument setup complete");
}

void loop() {
  if (Serial.available() > 0) {
    SCPI_Handler handler;
    handler.LoadString(Serial.readString());
    if (handler.ProcessCurrentCommand()) {
      Command command = handler.GetCurrentCommand();
      int argument = handler.GetCurrentArgument();
      core.SetPrinter(serial);
      core.ProcessCommand(command, argument); 
    }
  } 
  
  if (mqttClient.parseMessage()) {
    SCPI_Handler handler;
    handler.LoadString(mqttClient.readString());
    if (handler.ProcessCurrentCommand()) {
      Command command = handler.GetCurrentCommand();
      int argument = handler.GetCurrentArgument();
      core.SetPrinter(mqtt);
      core.ProcessCommand(command, argument); 
    }
  }
}
