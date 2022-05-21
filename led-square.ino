#include "wifi.h"
#include "patterns.h"
#include "state.h"
#include "mqtt.h"

void setup()
{
  Serial.begin(9600);
  setupFastLED();
  mqttSetup();
}

void loop()
{
  wifiLoop();
  mqttLoop();
  ledLoop();
}
