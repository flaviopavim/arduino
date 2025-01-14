#include "WiFiConnection.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>

const char *ssid = "Flavio";  // Nome do WiFi
const char *pass = "Rockandroll#";  // Senha do WiFi

ESP8266WiFiMulti WiFiMulti;
boolean wifiConnected = false;

void disconnectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from WiFi...");
    WiFi.disconnect();  // Desconecta da rede Wi-Fi atual
    WiFi.mode(WIFI_OFF);  // Desativa o modo Wi-Fi
    Serial.println("WiFi disconnected.");
  } else {
    Serial.println("WiFi is not connected.");
  }
}

boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}


/*
boolean connectWifi();
ESP8266WiFiMulti WiFiMulti;
boolean wifiConnected = false;

void disconnectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from WiFi...");
    WiFi.disconnect(); // Desconecta da rede Wi-Fi atual
    WiFi.mode(WIFI_OFF); // Desativa o modo Wi-Fi
    Serial.println("WiFi disconnected.");
  } else {
    Serial.println("WiFi is not connected.");
  }
}

boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}*/