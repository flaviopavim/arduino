#include "WiFiConnection.h"  // Includes the custom header for WiFi connection logic
#include <ESP8266WiFi.h>     // Core library for WiFi functionality on ESP8266
#include <ESP8266WiFiMulti.h> // Enables management of multiple WiFi connections
#include <WiFiClient.h>       // Provides client-side network capabilities

const char *ssid = "Flavio";  // WiFi network name (SSID)
const char *pass = "Rockandroll#";  // WiFi network password

ESP8266WiFiMulti WiFiMulti; // Object to handle multiple WiFi networks
boolean wifiConnected = false; // Tracks the connection status of the WiFi

// Function to disconnect from WiFi
void disconnectWifi() {
    if (WiFi.status() == WL_CONNECTED) { // Checks if currently connected
        Serial.println("Disconnecting from WiFi...");
        WiFi.disconnect();  // Disconnects from the current WiFi network
        WiFi.mode(WIFI_OFF);  // Turns off WiFi mode
        Serial.println("WiFi disconnected.");
    } else {
        Serial.println("WiFi is not connected."); // Informs if no connection exists
    }
}

// Function to connect to WiFi
boolean connectWifi() {
    boolean state = true; // Tracks the connection state
    int i = 0;            // Counter for retry attempts

    WiFi.mode(WIFI_STA);  // Sets the device to Station mode
    WiFi.begin(ssid, pass); // Initiates connection to the defined WiFi network
    Serial.println("");
    Serial.println("Connecting to WiFi");

    // Waits for connection with a timeout
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) { // Loops until connected
        delay(500);   // Waits for 500ms between retries
        Serial.print("."); // Prints a dot for each attempt
        if (i > 20) {  // Exits the loop after 20 failed attempts (~10 seconds)
            state = false;
            break;
        }
        i++;
    }
    Serial.println(""); // Adds a new line after the connection process

    // Displays the connection status
    if (state) { // If successfully connected
        Serial.print("Connected to ");
        Serial.println(ssid); // Prints the SSID of the connected network
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); // Prints the assigned IP address
    } else { // If connection failed
        Serial.println("Connection failed.");
    }
    return state; // Returns the connection state (true/false)
}
