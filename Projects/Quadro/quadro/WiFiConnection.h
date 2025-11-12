#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

extern const char *ssid;  // Nome do WiFi
extern const char *pass;  // Senha do WiFi

extern ESP8266WiFiMulti WiFiMulti;
extern boolean wifiConnected;

boolean connectWifi();
void disconnectWifi();

#endif
