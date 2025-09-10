#include "MqttClass.h"

#include <Arduino.h>

using namespace std;

/*
Load WiFi SSID & Pass and MQTT IP Address:
  const char* ssid = "WiFi_SSID";
  const char* pass = "WiFi_Pass";
  const char* mqttServer = "192.168.0.10";
*/
#include "networkCredentials.h"

//#define DEBUG_MODE
//#define DEBUG_MODE_MQTT

#define BAUDRATE 115200

#define RC_PIN 17

unsigned long mqttLastEvent;
unsigned long mqttInterval = 60000; //ms

int signalRC; // Ripple Control Signal (HDO)

const char* mqttPublishTopic = "/home/electro/switchboard_in";
const char* mqttSubscribeTopic = "/home/electro/switchboard_out";
const char* mqttSignalRCKey = "signal_RC";

void MqttCallback(char* topic, byte* message, unsigned long length);
MqttClass mqtt(mqttServer, MqttCallback);

void setup()
{
  delay(100);
  
  #ifdef DEBUG_MODE
  Serial.begin(BAUDRATE);
  while(!Serial);
  Serial.println();
  Serial.println("Switchboard");
  Serial.println();
  #endif

  #ifdef DEBUG_MODE
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  #endif

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    #ifdef DEBUG_MODE
    Serial.print(".");
    #endif
  }

  #ifdef DEBUG_MODE
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  #endif

  mqtt.setSubscribeTopic(mqttSubscribeTopic);
  mqtt.subscribe();
  mqtt.setPublishTopic(mqttPublishTopic);

  pinMode(RC_PIN, INPUT_PULLUP);

  mqttLastEvent = millis() - mqttInterval + 2000; //2seconds to call mqtt publish
  delay(500);
}

void loop()
{
  mqtt.loop();

  unsigned long now = millis();
  if(now - mqttLastEvent > mqttInterval)
  {
    mqttLastEvent = now;

    signalRC = !digitalRead(RC_PIN);

    #ifdef DEBUG_MODE
    Serial.println("Signal RC: " + String(signalRC));
    #endif

    mqtt.publish(mqttPublishTopic, mqttSignalRCKey, signalRC);
  }
}

void MqttCallback(char* topic, byte* message, unsigned long length)
{
  #ifdef DEBUG_MODE_MQTT
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  #endif

  String json;

  for(int i = 0; i < length; i++)
  {
    json += (char)message[i];
    #ifdef DEBUG_MODE_MQTT
    Serial.print((char)message[i]);
    #endif
  }
  #ifdef DEBUG_MODE_MQTT
  Serial.println();
  #endif

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);

  if(error)
  {
    #ifdef DEBUG_MODE_MQTT
    Serial.println();
    Serial.println("Json deserialization failed:");
    Serial.println(error.c_str());
    #endif
    return;
  }

  if(strcmp(topic, mqttSubscribeTopic) == 0)
  {
    if(!mqtt.getSynced())
    {
      mqtt.setSynced();
      #ifdef DEBUG_MODE_MQTT
      Serial.println("MQTT Synced: " + String(mqtt.getSynced()));
      #endif
    }
  }
}