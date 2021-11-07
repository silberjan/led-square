#include <WiFiNINA.h>
#include <MQTTClient.h>  
#include <ArduinoJson.h>
#include "arduino_secrets.h"

extern WiFiClient wifiClient;
extern MQTTClient mqttClient;

extern DynamicJsonDocument jsonIn;
extern DynamicJsonDocument jsonOut;

const int mqttPeriod = 1000;

extern unsigned long timeNowMQTT;

extern bool successfulFirstMessage;

void mqttLoop();

void handleMessage(String &topic, String &payload);

void sendHomeAssistantConfig();

void sendHomeAssistantState();

void initSubscription();

void mqttSetup();