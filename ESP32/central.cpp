/*

    Central ESP32

    Alarm, garden control

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Alexa
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>

#include <ArduinoJson.h>

String api = "";
const char *ssid=""; // Nome do WiFi
const char *pass=""; // Senha do WiFi
const char *uuid="abc123";   // ID registrado na API

// prototypes
boolean connectWifi();

//callback functions
void zeroLightChanged(uint8_t brightness);
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fifthLightChanged(uint8_t brightness);
//void sixthLightChanged(uint8_t brightness);
//void seventhLightChanged(uint8_t brightness);
//void eigthLightChanged(uint8_t brightness);

ESP8266WiFiMulti WiFiMulti;

String datetime="";
String command="";

// device names
String Device_0_Name = "esp 0";
String Device_1_Name = "esp 1";
String Device_2_Name = "esp 2";
String Device_3_Name = "esp 3";
String Device_4_Name = "esp 4";
String Device_5_Name = "esp 5";
//String Device_6_Name = "esp 6";
//String Device_7_Name = "esp 7";
//String Device_8_Name = "esp 8";

boolean wifiConnected = false;

Espalexa espalexa;

void lightChanged(int port, uint8_t brightness) {
  if (brightness) {
    if (brightness == 255) {
      digitalWrite(port, LOW);
      Serial.println(port);
      Serial.println("Device OFF");
    }
  } else {
    digitalWrite(port, HIGH);
    Serial.println(port);
    Serial.println("Device ON");
  }
}

void zeroLightChanged(uint8_t brightness) {
  lightChanged(D0,brightness);
}

void firstLightChanged(uint8_t brightness) {
  lightChanged(D1,brightness);
}

void secondLightChanged(uint8_t brightness) {
  lightChanged(D2,brightness);
}

void thirdLightChanged(uint8_t brightness) {
  lightChanged(D3,brightness);
}

void fourthLightChanged(uint8_t brightness) {
  lightChanged(D4,brightness);
}

void fifthLightChanged(uint8_t brightness) {
  lightChanged(D5,brightness);
}

//void sixthLightChanged(uint8_t brightness) {
//  lightChanged(R6,brightness);
//}

//void seventhLightChanged(uint8_t brightness) {
//  lightChanged(R7,brightness);
//}

//void eigthLightChanged(uint8_t brightness) {
//  if (brightness) {
//    if (brightness == 255) {
//        Serial.println("Alarm ON");
//    }
//  } else {
//      Serial.println("Alarm OFF");
//  }
//}

// connect to wifi – returns true if successful or false if not
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
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

/*
void rgb(int r, int g, int b) {
    analogWrite(R6, r);
    analogWrite(R7, g);
    analogWrite(R8, b);
}
*/

void setup() {

    Serial.begin(115200);
    Serial.println("Hello world");

    WiFiMulti.addAP(ssid, pass);

    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);

    //RGB
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    //rgb(255,255,0); //yellow
  
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    digitalWrite(D5, HIGH);
    //digitalWrite(R6, HIGH);
    //digitalWrite(R7, HIGH);
    //digitalWrite(R8, HIGH);
  
  
      // Initialise wifi connection
    wifiConnected = connectWifi();
  
    if (wifiConnected) {
        
        // Define your devices here.
        espalexa.addDevice(Device_0_Name, zeroLightChanged); //simplest definition, default state off
        espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
        espalexa.addDevice(Device_2_Name, secondLightChanged);
        espalexa.addDevice(Device_3_Name, thirdLightChanged);
        espalexa.addDevice(Device_4_Name, fourthLightChanged);
        espalexa.addDevice(Device_5_Name, fifthLightChanged);
        //espalexa.addDevice(Device_6_Name, sixthLightChanged);
        //espalexa.addDevice(Device_7_Name, seventhLightChanged);
        //espalexa.addDevice(Device_8_Name, eigthLightChanged);
        
        espalexa.begin();
  
    } else {
        while (1) {
            Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
            delay(2500);
        }
    }
}

bool bool_water=false;
int count_water=0;

int count=0;
void loop() {
    espalexa.loop();

    //jsonn();
  

    if (count==100) { //em média a cada 10 segundos
      count=0;
      if (WiFiMulti.run() == WL_CONNECTED) {

        WiFiClient client;
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        if (http.begin(client, api)) {  // HTTP
          Serial.print("[HTTP] POST...\n");

          // Set content type to JSON
          http.addHeader("Content-Type", "application/json");

          // Create a StaticJsonDocument
          StaticJsonDocument<200> jsonDocument;

          // Fill in the JSON fields
          jsonDocument["from"] = "cpp";
          jsonDocument["to"] = "php";
          jsonDocument["system"] = "security";
          jsonDocument["version"] = "1.0.0";
          jsonDocument["uuid"] = "esp32";

          // Create a nested JSON object for the "data" field
          JsonObject nestedData = jsonDocument.createNestedObject("data");
          nestedData["view"] = "refresh";

          // Serialize the JSON object to a string
          String jsonString;
          serializeJson(jsonDocument, jsonString);

          // Print the serialized JSON string
          Serial.println(jsonString);

          int httpCode = http.POST(jsonString);
          if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {

              String get = http.getString();
              Serial.println(get);

              const char* json = get.c_str();
              
              JsonDocument doc;
              DeserializationError error = deserializeJson(doc, json);

              if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
              }

              //const char* from = doc["from"];
              //const char* to = doc["to"];
              //const char* system = doc["system"];
              //const char* version = doc["version"];
              const char* status = doc["status"];
              //const char* msg = doc["msg"];

              if (status=="success") {

              }

              /*

              command=get.substring(20,get.length());
              datetime=get.substring(0,19);

              Serial.println(datetime);
              Serial.println(command);


              if (command=="water") {
                bool_water=true;
              }

              String year  = datetime.substring(0,4);
              String month = datetime.substring(6,7);
              String day   = datetime.substring(9,10);
              String hour  = datetime.substring(12,13);
              String min   = datetime.substring(15,16);
              String sec   = datetime.substring(18,19);

              //liga a água às 05:00, 11:00 e 17:00
              if ((hour=="05" || hour=="11" || hour=="17") && min=="00") {
                bool_water=true;
              }

              if (bool_water) {

                //rgb(0,255,0); //green

                if (count_water==0) {
                    //liga a água
                    digitalWrite(D2, LOW);
                    Serial.println("Device2 ON");
                }

                count_water++;

                //desliga a água em 2 minutos
                //a função é puxada a cada 10 segundos
                //60 segundos = 6 x 10 segundos
                //2 x 60 segundos = 2 minutos
                //if (count_water==6*2) {
                if (count_water==6) { // 1 minuto
                    //reinicia o contador
                    count_water=0;
                    bool_water=false;
                    //desliga a água
                    digitalWrite(D2, HIGH);
                    Serial.println("Device2 OFF");
                }

              }
              */

            }
          } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
          http.end();
        } else {
          Serial.printf("[HTTP] Unable to connect\n");
        }
      }
    }
    count++;
    delay(100);
}