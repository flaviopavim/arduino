#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

String api = "http://flaviopavim.com.br/api/datetime.php";

const char *ssid = "Flavio";      // Nome do WiFi
const char *pass = "Rockandroll#"; // Senha do WiFi
ESP8266WiFiMulti WiFiMulti;
boolean wifiConnected = false;

boolean connectWifi() {
    boolean state = true;
    int i = 0;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.println("");
    Serial.println("Connecting to WiFi");

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



// Exemplo de função para lidar com a API
String getDataFromApi() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(api);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
            return payload;
        } else {
            Serial.println("Error on HTTP request");
        }
        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
    return "";
}
