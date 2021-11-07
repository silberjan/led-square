#include <OneButton.h>
#include "wifi.h"
#include "patterns.h"
#include "state.h"
#include "mqtt.h"

#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

void setup()
{
  Serial.begin(9600);
  btn.attachClick(btnClicked);
  btn.attachDoubleClick(btnDblClicked);
  mqttClient.setBufferSize(512);
  randomSeed(analogRead(0));
  setupFastLED();
}

void loop()
{
  wifiLoop();
  mqttLoop();
  ledLoop();
  btn.tick();
}

void btnClicked()
{
  nextPattern();
}

void btnDblClicked() {
  led_on = !led_on;
  sendHomeAssistantState();
}

