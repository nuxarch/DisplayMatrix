#pragma once
#include <Arduino.h>
#include "stdlib.h"
#include <PubSubClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient client(espClient);
Preferences preferences;

char *inputMessage = "";
char *payloadAsChar = "";
bool publishInputMessage = false;

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char *mqtt_ID = "mqttx_fat";
// MQTT Credentials
const char *mqtt_username = "fat";
const char *mqtt_password = "aser";
const char *mqtt_client = "ESP32A01";

int reconnectCounter = 0;
long lastReconnectAttempt = 0;

void publishMQTT(char *topic, char *payload);
StaticJsonDocument<200> doc;
const char *text = "";
void callback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0';
    char *payloadAsChar = (char *)payload;
    char *topicID = strtok(topic, "/");
    char *topicFunction = strtok(NULL, "/");
    Serial.println("ID:" + String(topicID) + ", Function:" + String(topicFunction));
    DeserializationError error = deserializeJson(doc, payloadAsChar);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    text = doc["Text"];
    // Message1 = "" + String(wd[dow]) + ", " + String(d) + " " + String(months[month]) + " " + String(yr) + " PRODI TPPU";
    Serial.println("Text:" + String(text));
    if (strcmp(topicFunction, "set") == 0)
    {
        // publishMQTT("reply", (char *)text);
    }
    
}
class mqqt
{
private:
    /* data */
public:
    mqqt(/* args */);
    ~mqqt();

    char dataArray[30];
    void get_mqqt_text(){
        
    }
    void publishMQTT(char *topic, char *payload)
    {
        snprintf(dataArray, sizeof dataArray, "%s/%s", mqtt_ID, topic);
        char *result = (char *)malloc(strlen(dataArray) + 1); // Copy char array to a char pointer so return a pointer
        strcpy(result, dataArray);
        client.publish(result, payload);
        Serial.print(result);
        Serial.print("/");
        Serial.println(payload);
    }

    void subscribeMQTT(char *topic)
    {
        char dataArray[30];
        snprintf(dataArray, sizeof dataArray, "%s/%s", mqtt_ID, topic);
        char *result = (char *)malloc(strlen(dataArray) + 1); // Copy char array to a char pointer so return a pointer
        strcpy(result, dataArray);
        client.subscribe(result);
        Serial.println(result);
    }

    void publishSystem()
    {
        publishMQTT("wifi", Wifi.getRssiAsQuality());
        // publishMQTT("uptime", Time.getUptime());
    }

    void reSubscribe()
    {
        subscribeMQTT("set");
        Serial.println("Connected, subscribing... ");
    }

    void reconnect()
    {
        Serial.println("Attempting MQTT connection... ");
        if (client.connect(mqtt_client, mqtt_username, mqtt_password))
        {
            reSubscribe();
        }
        else if (reconnectCounter > 5)
        {
            Serial.println("Resetting ESP32");
            delay(500);
            // ESP.restart();
        }
        else
        {
            reconnectCounter++;
            Serial.print("Attempt: ");
            Serial.print(reconnectCounter);
            Serial.print(" failed, Error: ");
            Serial.print(client.state());
            Serial.print(" Retrying in 5 seconds");
        }
    }
};

mqqt::mqqt(/* args */)
{
}

mqqt::~mqqt()
{
}

extern mqqt myMQQT;