
#ifndef API_H
#define API_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Espalexa.h>
#include <ArduinoJson.h>

extern String api;
extern bool bool_get_hour;
extern int hours, minutes, seconds;
extern int hour1, hour2, minute1, minute2, second1, second2;

extern String color;
extern String phrase;
extern String bitcoin;
extern String wallet;
extern String usd;

void setTime();

#endif
