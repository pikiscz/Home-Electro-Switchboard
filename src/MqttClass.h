#ifndef MQTTCLASS_H
#define MQTTCLASS_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

class MqttClass
{
private:
    const char* _mqttServer;
    const char* _subscribeTopic;
    const char* _publishTopic;

    bool _connected = false;
    unsigned long _reconnectLastEvent;
    unsigned long _reconnectInterval = 5000;
    bool _synchoronized;

    WiFiClient _espClient;
    PubSubClient _client;

    void SerializeDoc(const char* topic, JsonDocument doc);

public:
    MqttClass(const char* mqttServer);
    MqttClass(
        const char* mqttServer,
        std::function<void(char*, uint8_t*, unsigned int)> mqttCallback
    );
    ~MqttClass();
    //Return MQTT IP Address
    inline String getMqttServerIP() {return String(_mqttServer); }
    //Set callback function to receive messages
    void setCallback(std::function<void(char*, uint8_t*, unsigned int)> mqttCallback);
    //Set topic to be subscribed for callback function
    void setSubscribeTopic(const char* topic);
    //Set publishing topic
    void setPublishTopic(const char* topic);
    //Return publishing topic
    inline const char* getPublishTopic() { return _publishTopic; }
    //Subscribe to all topics set by subscribeTopics()
    void subscribe();
    //Try to connect to MQTT server with random client ID
    //Return TRUE when succesfully connected, othewise return FALSE
    bool reconnect();
    //Loop funciton checks for connection status and incomming messages
    void loop();
    //Return TRUE when connected to MQTT server
    inline bool getConnected() { return _connected; }
    //Return synchornizetion status
    //Status is set via getSynced()
    inline bool getSynced() { return _synchoronized; }
    //Set synchronizetion status to TRUE
    inline void setSynced() { _synchoronized = true; }
    //Set synchonization status
    inline void setSynced( bool synchornized) { _synchoronized = synchornized; }
    //Publish to MQTT server
    void publish(
        const char* topic,
        const char* key1, float value1
    );
    //Publish to MQTT server
    void publish(
        const char* topic,
        const char* key1, float value1,
        const char* key2, float value2
    );
    //Publish to MQTT server
    void publish(
        const char* topic,
        const char* key1, float value1,
        const char* key2, float value2,
        const char* key3, float value3
    );
    //Publish to MQTT server
    void publish(
        const char* topic,
        const char* key1, float value1,
        const char* key2, float value2,
        const char* key3, float value3,
        const char* key4, float value4
    );
};

#endif