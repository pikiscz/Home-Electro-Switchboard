#include "MqttClass.h"

MqttClass::MqttClass(const char* mqttServer)
{
    _mqttServer = mqttServer;
    
    _client.setClient(_espClient);
    _client.setServer(_mqttServer, 1883);
}

MqttClass::MqttClass(
    const char* mqttServer,
    std::function<void(char*, uint8_t*, unsigned int)> mqttCallback
)
{
    _mqttServer = mqttServer;
    
    _client.setClient(_espClient);
    _client.setServer(_mqttServer, 1883);
    _client.setCallback(mqttCallback);
}

MqttClass::~MqttClass() {}

void MqttClass::setCallback(std::function<void(char*, uint8_t*, unsigned int)> mqttCallback)
{
    _client.setCallback(mqttCallback);
}

void MqttClass::setSubscribeTopic(const char* topic)
{
    _subscribeTopic = topic;
}

void MqttClass::setPublishTopic(const char* topic)
{
    _publishTopic = topic;
}

void MqttClass::subscribe()
{
    _client.subscribe(_subscribeTopic);
}

bool MqttClass::reconnect()
{
    if(!_client.connected())
    {
        String clientId = "ESP32Client-LVR-";
        clientId += String(random(0xffff), HEX);

        if(_client.connect(clientId.c_str()))
        {
            subscribe();
            _connected = true;
            publish(_publishTopic, "connected", true);
        }
        else
        {
            _connected = false;
        }
    }
    return _connected;
}

void MqttClass::loop()
{
    unsigned long now = millis();
    if(!_client.connected())
    {
        if(now - _reconnectLastEvent > _reconnectInterval)
        {
            _reconnectLastEvent = now;
            reconnect();
        }
    }

    _client.loop();
}

void MqttClass::publish(
    const char* topic,
    const char* key1, float value1
)
{
    JsonDocument doc;
    doc[key1] = value1;
    SerializeDoc(topic, doc);
}

void MqttClass::publish(
    const char* topic,
    const char* key1, float value1,
    const char* key2, float value2
)
{
    JsonDocument doc;
    doc[key1] = value1;
    doc[key2] = value2;
    SerializeDoc(topic, doc);
}

void MqttClass::publish(
    const char* topic,
    const char* key1, float value1,
    const char* key2, float value2,
    const char* key3, float value3
)
{
    JsonDocument doc;
    doc[key1] = value1;
    doc[key2] = value2;
    doc[key3] = value3;
    SerializeDoc(topic, doc);
}

void MqttClass::publish(
    const char* topic,
    const char* key1, float value1,
    const char* key2, float value2,
    const char* key3, float value3,
    const char* key4, float value4
)
{
    JsonDocument doc;
    doc[key1] = value1;
    doc[key2] = value2;
    doc[key3] = value3;
    doc[key4] = value4;
    SerializeDoc(topic, doc);
}

void MqttClass::SerializeDoc(const char* topic, JsonDocument doc)
{
    char output[100];
    serializeJson(doc, output);
    _client.publish(topic, output);
}