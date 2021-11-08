#include "wifi.h"
#include "patterns.h"
#include "state.h"
#include "mqtt.h"

#define BTN_PIN 7

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  setupFastLED();
  mqttSetup();
}

void loop()
{
  wifiLoop();
  mqttLoop();
  ledLoop();
}
