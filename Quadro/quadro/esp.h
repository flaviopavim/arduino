#ifndef ESP_H
#define ESP_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

extern const char *ssid;
extern const char *pass;

boolean connectWifi();
void disconnectWifi();

#endif
