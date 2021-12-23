#include <SPI.h>
#include "mqtt.h"
#include "state.h"

unsigned long timeNowMQTT = 0;

MQTTClient mqttClient(512);
WiFiClient wifiClient;

DynamicJsonDocument jsonIn(512);
DynamicJsonDocument jsonOut(512);
DynamicJsonDocument configJson(512);
char configJsonBuf[512];
size_t configJsonSize;

bool successfulFirstMessage = false;
bool sendStateInNextLoop = false;

void mqttSetup()
{
  configJson["~"] = "homeassistant/light/led-square";
  configJson["name"] = "LED Square";
  configJson["unique_id"] = "led-square";
  configJson["cmd_t"] = "~/set";
  configJson["stat_t"] = "~/state";
  configJson["schema"] = "json";
  configJson["brightness"] = true;
  configJson["color_mode"] = true;
  configJson["supported_color_modes"][0] = "rgb";
  configJson["effect"] = true;
  configJson["effect_list"][0] = "static";
  configJson["effect_list"][1] = "rain";
  configJson["effect_list"][2] = "rain_reverse";
  configJson["effect_list"][3] = "grow";
  configJson["effect_list"][4] = "noise";
  configJson["effect_list"][5] = "flash";

  configJsonSize = serializeJson(configJson, configJsonBuf);

  mqttClient.begin("192.168.2.150", 1883, wifiClient);
  mqttClient.onMessage(handleMessage);
}

void mqttLoop()
{
  if (wifiStatus == WL_CONNECTED)
  {
    mqttClient.loop();

    if (sendStateInNextLoop)
    {
      sendHomeAssistantState();
    }

    if (millis() > timeNowMQTT + mqttPeriod)
    {
      timeNowMQTT = millis();
      if (!mqttClient.connected())
      {
        connectMqtt();
      }
    }
  }
}

void connectMqtt()
{
  successfulFirstMessage = false;
  Serial.print("Connecting MQTT");
  const char clientId[] = "ledsquare-0";
  if (mqttClient.connect(clientId))
  {
    Serial.print("\nMQTT Connection successful. Client ID: ");
    Serial.println(clientId);
    initSubscription();
  }
}

void initSubscription()
{
  const bool sub = mqttClient.subscribe(subscribeTopic);
  Serial.print("Subscribed ");
  Serial.println(sub);
  if (sub)
  {
    sendHomeAssistantConfig();
    sendHomeAssistantState();
  }
}

void sendHomeAssistantConfig()
{
  char topic[] = "homeassistant/light/led-square/config";
  boolean rc = mqttClient.publish(topic, configJsonBuf, configJsonSize, true, 0);
  Serial.print("Sent msg ");
  Serial.println(rc);
  if (rc)
  {
    sendHomeAssistantState();
  }
}

void sendHomeAssistantState()
{
  sendStateInNextLoop = false;
  char topic[] = "homeassistant/light/led-square/state";
  jsonOut.clear();
  jsonOut["state"] = led_on ? "ON" : "OFF";
  if (led_on)
  {
    jsonOut["color_mode"] = "rgb";
    jsonOut["brightness"] = led_brightness;
    jsonOut["color"]["r"] = led_r;
    jsonOut["color"]["g"] = led_g;
    jsonOut["color"]["b"] = led_b;
    jsonOut["effect"] = getPattenName();
  }
  char buffer[512];
  size_t s = serializeJson(jsonOut, buffer);
  boolean rc = mqttClient.publish(topic, buffer, s, true, 0);
  Serial.print("Sent state msg ");
  Serial.println(rc);
  Serial.println(buffer);
}

void handleMessage(String &topic, String &payload)
{
  if (!successfulFirstMessage)
  {
    successfulFirstMessage = true;
  }
  Serial.print("Received message ");
  Serial.println(topic);
  if (topic == "homeassistant/light/led-square/set")
  {
    Serial.println("Received set message");
    Serial.println(payload);
    deserializeJson(jsonIn, payload);
    if (jsonIn["state"])
    {
      led_on = jsonIn["state"] == "ON";
    }
    if (jsonIn["brightness"])
    {
      led_brightness = jsonIn["brightness"];
    }
    if (jsonIn["color"])
    {
      led_r = jsonIn["color"]["r"];
      led_g = jsonIn["color"]["g"];
      led_b = jsonIn["color"]["b"];
      led_rgb = CRGB(led_r, led_g, led_b);
      led_hsv = rgb2hsv_approximate(led_rgb);
    }
    if (jsonIn["effect"])
    {
      const String effect = jsonIn["effect"];
      if (effect == "static")
      {
        currentPattern = STATIC;
      }
      else if (effect == "rain")
      {
        currentPattern = RAIN;
      }
      else if (effect == "rain_reverse")
      {
        currentPattern = RAIN_REVERSE;
      }
      else if (effect == "grow")
      {
        currentPattern = GROW;
      }
      else if (effect == "noise")
      {
        currentPattern = NOISE;
      }
      else if (effect == "flash")
      {
        currentPattern = FLASH;
      }
    }
    sendStateInNextLoop = true;
  }
}
