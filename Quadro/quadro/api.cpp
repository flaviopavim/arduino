#include "api.h"
#include <ESP8266HTTPClient.h>

void fetchApiData() {
    HTTPClient http;
    http.begin("http://flaviopavim.com.br/api/datetime.php");
    int httpCode = http.GET();

    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println("API Response: " + payload);
    } else {
        Serial.println("Failed to fetch API data");
    }

    http.end();
}

