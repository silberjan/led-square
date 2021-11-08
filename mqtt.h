#include <WiFiNINA.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

extern WiFiClient wifiClient;
extern MQTTClient mqttClient;

extern DynamicJsonDocument jsonIn;
extern DynamicJsonDocument jsonOut;
extern DynamicJsonDocument configJson;
extern char configJsonBuf[512];
extern size_t configJsonSize;

const int mqttPeriod = 1000;

extern unsigned long timeNowMQTT;

extern bool successfulFirstMessage;
extern bool sendStateInNextLoop;

const char subscribeTopic[] = "homeassistant/light/led-square/#";

void mqttLoop();

void handleMessage(String &topic, String &payload);

void sendHomeAssistantConfig();

void sendHomeAssistantState();

void initSubscription();

void mqttSetup();

void connectMqtt();