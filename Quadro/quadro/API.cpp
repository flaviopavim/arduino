#include "WiFiConnection.h"  // Include WiFi connection utilities
#include "API.h"             // Include API utilities

String api = "http://flaviopavim.com.br/api/now.php";  // URL to fetch the current time

bool bool_get_hour = false;  // Flag to indicate if the time was successfully retrieved

int hours = 0;   // Variable to store the hour
int minutes = 0; // Variable to store the minutes
int seconds = 0; // Variable to store the seconds

// Variables to store individual digits of hours, minutes, and seconds
int hour1 = 0;
int hour2 = 0;
int minute1 = 0;
int minute2 = 0;
int second1 = 0;
int second2 = 0;

char* bitcoin="000000";

void setTime() {
  WiFiMulti.addAP(ssid, pass);  // Add WiFi access point
  delay(2);                     // Small delay for stability
  disconnectWifi();             // Disconnect any existing WiFi connections
  wifiConnected = connectWifi();  // Attempt to connect to WiFi
  delay(15);                    // Delay to allow connection

  if (wifiConnected) {  // Check if WiFi is connected
    Serial.println("Fetching current time...");
    
    if (WiFiMulti.run() == WL_CONNECTED) {  // Verify WiFi connection is active
      WiFiClient client;   // Create a WiFi client
      HTTPClient http;     // Create an HTTP client
      Serial.print("[HTTP] begin...\n");
      
      if (http.begin(client, api)) {  // Initialize HTTP connection
        http.addHeader("Content-Type", "application/json");  // Add header for JSON content
        int httpCode = http.GET();  // Perform HTTP GET request
        
        if (httpCode > 0) {  // Check if HTTP request was successful
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();  // Get response payload
            StaticJsonDocument<256> doc;       // Create a JSON document
            DeserializationError error = deserializeJson(doc, payload);  // Parse JSON response

            if (!error) {  // Check if JSON parsing was successful


              String bitcoinString = doc["bitcoin"];  // Extrai o valor de "bitcoin" da resposta JSON
              bitcoin = strdup(bitcoinString.c_str());  // Converte a String para char* e aloca memória dinamicamente

              String datetime = doc["datetime"]; // Extract datetime string (e.g., "2025-01-12 14:02:39")
              int spacePos = datetime.indexOf(' ');  // Find space separating date and time
              
              if (spacePos != -1) {  // Ensure space was found
                String timePart = datetime.substring(spacePos + 1);  // Extract time part (e.g., "14:02:39")
                
                int firstColon = timePart.indexOf(':');  // Locate first colon in time string
                int secondColon = timePart.indexOf(':', firstColon + 1);  // Locate second colon
                
                if (firstColon != -1 && secondColon != -1) {  // Ensure colons were found
                  hours = timePart.substring(0, firstColon).toInt();  // Extract hours
                  minutes = timePart.substring(firstColon + 1, secondColon).toInt();  // Extract minutes
                  seconds = timePart.substring(secondColon + 1).toInt();  // Extract seconds

                  // Extract individual digits for hours, minutes, and seconds
                  hour1 = hours / 10;
                  hour2 = hours % 10;
                  minute1 = minutes / 10;
                  minute2 = minutes % 10;
                  second1 = seconds / 10;
                  second2 = seconds % 10;

                  // Print extracted values for debugging
                  Serial.print("Time: ");
                  Serial.print(hour1);
                  Serial.print(hour2);
                  Serial.print(":");
                  Serial.print(minute1);
                  Serial.print(minute2);
                  Serial.print(":");
                  Serial.print(second1);
                  Serial.println(second2);
                }
              }

              bool_get_hour = true;  // Indicate successful time retrieval
            }
          }
        }
      }
    }
  }
}
