#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Define the API endpoint
String api = "http://192.168.69.101/";

// Define WiFi credentials
const char *wifi = "kicko";   // WiFi network name (SSID)
const char *pass = "qweasd01"; // WiFi password

// WiFiMulti allows connecting to multiple access points
ESP8266WiFiMulti WiFiMulti;

void setup() {
    // Initialize the serial monitor for debugging
    Serial.begin(115200);

    // Set WiFi mode to station (client)
    WiFi.mode(WIFI_STA);

    // Add the WiFi credentials
    WiFiMulti.addAP(wifi, pass);

    // Configure the pins (A0 and D0 to D8) as outputs
    pinMode(A0, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    // Set all pins to HIGH (turn them off initially)
    digitalWrite(A0, HIGH);
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D8, HIGH);
}

void loop() {
    // Check if the device is connected to WiFi
    if (WiFiMulti.run() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        // Debug message for starting the HTTP request
        Serial.print("[HTTP] begin...\n");

        // Start the HTTP GET request to the specified API endpoint
        if (http.begin(client, api + "base/esp/")) {
            Serial.print("[HTTP] GET...\n");

            // Send the GET request
            int httpCode = http.GET();

            // Check if the request was successful
            if (httpCode > 0) {
                Serial.printf("[HTTP] GET... code: %d\n", httpCode);

                // If the response is OK, process it
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                    String payload = http.getString(); // Get the response payload
                    Serial.println(payload);          // Debug: print the payload

                    // Compare the payload with expected commands to control pins
                    if (payload == "A01") {
                        digitalWrite(A0, LOW);
                    } else if (payload == "A00") {
                        digitalWrite(A0, HIGH);
                    } else if (payload == "D01") {
                        digitalWrite(D0, LOW);
                    } else if (payload == "D00") {
                        digitalWrite(D0, HIGH);
                    } else if (payload == "D11") {
                        digitalWrite(D1, LOW);
                    } else if (payload == "D10") {
                        digitalWrite(D1, HIGH);
                    } else if (payload == "D21") {
                        digitalWrite(D2, LOW);
                    } else if (payload == "D20") {
                        digitalWrite(D2, HIGH);
                    } else if (payload == "D31") {
                        digitalWrite(D3, LOW);
                    } else if (payload == "D30") {
                        digitalWrite(D3, HIGH);
                    } else if (payload == "D41") {
                        digitalWrite(D4, LOW);
                    } else if (payload == "D40") {
                        digitalWrite(D4, HIGH);
                    } else if (payload == "D51") {
                        digitalWrite(D5, LOW);
                    } else if (payload == "D50") {
                        digitalWrite(D5, HIGH);
                    } else if (payload == "D61") {
                        digitalWrite(D6, LOW);
                    } else if (payload == "D60") {
                        digitalWrite(D6, HIGH);
                    } else if (payload == "D71") {
                        digitalWrite(D7, LOW);
                    } else if (payload == "D70") {
                        digitalWrite(D7, HIGH);
                    } else if (payload == "D81") {
                        digitalWrite(D8, LOW);
                    } else if (payload == "D80") {
                        digitalWrite(D8, HIGH);
                    }
                }
            } else {
                // Print the error if the GET request failed
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }

            // End the HTTP connection
            http.end();
        } else {
            // Print an error if unable to connect
            Serial.printf("[HTTP] Unable to connect\n");
        }
    }

    // Delay for 1 second before the next loop iteration
    delay(1000);
}
