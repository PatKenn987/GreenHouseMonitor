#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
//#include <OneWire.h>
#include <DallasTemperature.h>

//Variable to store the number of temperature probes in the system.
int DevNum = 0;

//MQTT Broker
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "192.168.1.233";
const char* MqttClientID = "ESP8266Client";

char msg[75];

// Replace with your network details
//const char* ssid = "FiOS-M07WY-Guest";
//const char* password = "Kennedy987";
const char* ssid = "FiOS-M07WY";
const char* password = "toe6723ear0906rock";

const char* user_name = "PJK";
const char* pass = "SkiHaus";

//Main Loop Timing
long lastMsg = 0;
int value = 0;

WiFiClient espClient;

PubSubClient client(mqtt_server, 1883, espClient);

//------------------------------------------
void setup() {
  //Setup Serial port speed
  Serial.begin(115200);
  //Setup DS18b20 temperature sensor
  SetupDS18B20();
  DevNum = GetNumOfTempDevices();

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  long now = millis();
  //MQTT Loop
  if(!client.connected()) {
    reconnect();
  }
  client.loop();
  
  TempLoop( now );
/*
  for(int i=0; i<DevNum; i++){
      Serial.print("Temp");
      Serial.print(i);
      Serial.print("= ");
      Serial.println(GetCurrentTemp(i));
   }*/

    if(now-lastMsg >2000){
      lastMsg = now;
      ++value;
      //snprintf (msg, 75, "hello word #%ld", value);
      snprintf(msg,75,"testing/mqtt/topic = #%f", GetCurrentTemp(0));
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("TempOut",msg);
  }
  delay(3000);  
}
