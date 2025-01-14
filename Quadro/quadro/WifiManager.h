#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

extern const char *ssid;
extern const char *pass;
extern ESP8266WiFiMulti WiFiMulti;

void disconnectWifi();
boolean connectWifi();

#endif