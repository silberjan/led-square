#include <SPI.h>
#include "mqtt.h"
#include "state.h"

unsigned long timeNowMQTT = 0;

MQTTClient mqttClient(512);
WiFiClient wifiClient;

DynamicJsonDocument jsonIn(1024);
DynamicJsonDocument jsonOut(512);

bool successfulFirstMessage = false;

void mqttSetup()
{
  mqttClient.begin("192.168.2.150", 1883, wifiClient);
  mqttClient.onMessage(handleMessage);
}

void mqttLoop()
{
  if (wifiStatus == WL_CONNECTED && !mqttClient.connected() && millis() > timeNowMQTT + mqttPeriod)
  {
    timeNowMQTT = millis();
    successfulFirstMessage = false;
    Serial.println("Connecting MQTT...");
    String clientId = "ledsquare-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str()))
    {
      Serial.print("MQTT Connection successful. Client ID: ");
      Serial.println(clientId.c_str());
      initSubscription();
    }
    else
    {
      Serial.println("MQTT Connection failed");
    }
  }
  mqttClient.loop();
}

void initSubscription()
{
  mqttClient.unsubscribe("homeassistant/light/led-square/#");
  bool sub = mqttClient.subscribe("homeassistant/light/led-square/#");
  Serial.print("Subscribed ");
  Serial.println(sub);
  sendHomeAssistantState();
}

void sendHomeAssistantConfig()
{
  char topic[] = "homeassistant/light/led-square/config";
  const char payload[] = "{ \"~\": \"homeassistant/light/led-square\", \"name\": \"LED Square\", \"unique_id\": \"led-square\", \"cmd_t\": \"~/set\", \"stat_t\": \"~/state\", \"schema\": \"json\", \"brightness\": true, \"color_mode\": true, \"supported_color_modes\": [\"rgb\"]}";
  boolean rc = mqttClient.publish(topic, payload, true, 0);
  Serial.print("Sent msg ");
  Serial.println(rc);
  if (rc)
  {
    sendHomeAssistantState();
  }
}

void sendHomeAssistantState()
{
  char topic[] = "homeassistant/light/led-square/state";
  jsonOut.clear();
  jsonOut["state"] = led_on ? "ON" : "OFF";
  if (led_on)
  {
    jsonOut["brightness"] = led_brightness;
    jsonOut["color"]["r"] = led_r;
    jsonOut["color"]["g"] = led_g;
    jsonOut["color"]["b"] = led_b;
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
  successfulFirstMessage = true;
  Serial.print("Received message ");
  Serial.println(topic);
  if (topic == "homeassistant/light/led-square/set")
  {
    Serial.println("Received set message");
    deserializeJson(jsonIn, payload);
    led_on = jsonIn["state"] == "ON";
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
    sendHomeAssistantState();
  }
}
