#include "esp.h"

ESP8266WiFiMulti WiFiMulti;

const char *ssid = "Flavio";
const char *pass = "Rockandroll#";

boolean connectWifi() {
    boolean state = true;
    int i = 0;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("");
    Serial.println("Connecting to WiFi");

    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (i > 20) {
            state = false;
            break;
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

void disconnectWifi() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Disconnecting from WiFi...");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        Serial.println("WiFi disconnected.");
    } else {
        Serial.println("WiFi is not connected.");
    }
}
