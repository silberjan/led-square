#include <WiFiNINA.h>
#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASSWORD;

const int period = 10000;

extern unsigned long timeNowWifi;

extern bool attempted;

void wifiLoop();

void printWifiData();