#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

extern WiFiClient mqttWifiClient;
extern PubSubClient mqttClient;

extern DynamicJsonDocument jsonIn;
extern DynamicJsonDocument jsonOut;

const int mqttPeriod = 1000;

extern unsigned long timeNowMQTT;

extern bool successfulFirstMessage;


void mqttLoop();

void handleMessage(char* topic, byte* payload, unsigned int length);

void sendHomeAssistantConfig();

void sendHomeAssistantState();

void initSubscription();