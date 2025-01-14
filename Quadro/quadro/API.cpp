#include "WiFiConnection.h"
#include "API.h"

String api = "http://flaviopavim.com.br/api/now.php";

bool bool_get_hour = false;

int hours = 0;
int minutes = 0;
int seconds = 0;

int hour1 = 0;
int hour2 = 0;
int minute1 = 0;
int minute2 = 0;
int second1 = 0;
int second2 = 0;

void setTime() {
  WiFiMulti.addAP(ssid, pass);
  delay(2);
  disconnectWifi();
  wifiConnected = connectWifi();
  delay(15);

  if (wifiConnected) {
    Serial.println("Buscando hora certa...");
    
    if (WiFiMulti.run() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      Serial.print("[HTTP] begin...\n");
      
      if (http.begin(client, api)) {  // HTTP
        http.addHeader("Content-Type", "application/json");
        int httpCode = http.GET();
        
        if (httpCode > 0) {
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            StaticJsonDocument<256> doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
              String datetime = doc["datetime"]; // "2025-01-12 14:02:39"
              int spacePos = datetime.indexOf(' ');  // Localiza o espaço entre data e hora
              
              if (spacePos != -1) {
                String timePart = datetime.substring(spacePos + 1);  // Obtém a parte da hora (22:33:51)
                
                int firstColon = timePart.indexOf(':');
                int secondColon = timePart.indexOf(':', firstColon + 1);
                
                if (firstColon != -1 && secondColon != -1) {
                  hours = timePart.substring(0, firstColon).toInt();  // "22"
                  minutes = timePart.substring(firstColon + 1, secondColon).toInt();  // "33"
                  seconds = timePart.substring(secondColon + 1).toInt();  // "51"

                  // Extraindo os dígitos separadamente
                  hour1 = hours / 10;
                  hour2 = hours % 10;
                  minute1 = minutes / 10;
                  minute2 = minutes % 10;
                  second1 = seconds / 10;
                  second2 = seconds % 10;

                  // Imprimindo os valores para depuração
                  Serial.print("Hora: ");
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

              bool_get_hour = true;
            }
          }
        }
      }
    }
  }
}
