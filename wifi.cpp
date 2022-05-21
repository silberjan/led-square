#include <SPI.h>
#include "wifi.h"
#include "state.h"

unsigned long timeNowWifi = 0;
bool attempted = false;

void wifiLoop()
{
  if (millis() > timeNowWifi + period)
  {
    wifiStatus = WiFi.status();
    // printWifiData();
    timeNowWifi = millis();
    if (wifiStatus != WL_CONNECTED)
    {
      led_brightness = 8U;
      setColor(255, 0, 0);
      attempted = true;
      Serial.print("Attempting to connect to network: ");
      Serial.println(ssid);
      wifiStatus = WiFi.begin(ssid, pass);
    }
    if (wifiStatus == WL_CONNECTED)
    {
      if (attempted)
      {
        attempted = false;
        led_brightness = 16U;
        setColor(255, 255, 0);
        Serial.println("You're connected to the network");
        Serial.println("----------------------------------------");
        printWifiData();
        Serial.println("----------------------------------------");
      }
    }
  }
}

void printWifiData()
{
  IPAddress ip = WiFi.localIP();
  Serial.print("Wifi Status: ");
  Serial.println(wifiStatus);
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.println(rssi);
}