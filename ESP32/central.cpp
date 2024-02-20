/*

    Central ESP32

    Alarm, garden control

    API UNDER CONSTRUCTION - COMING SOON

    D1 = Alarm
    D2 = Computer
    D3 = Water
    D4 = 
    D5 = 
    D6 = R
    D7 = G
    D8 = B

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

String api = "http://whitehats.com.br/api/security/";
const char *ssid=""; // Nome do WiFi
const char *pass=""; // Senha do WiFi
const char *uuid="abc123";   // ID registrado na API

// prototypes
boolean connectWifi();

//callback functions
void alarmChanged(uint8_t brightness);
void computerChanged(uint8_t brightness);
void waterChanged(uint8_t brightness);
//void thirdLightChanged(uint8_t brightness);
//void fourthLightChanged(uint8_t brightness);
//void fifthLightChanged(uint8_t brightness);

ESP8266WiFiMulti WiFiMulti;

String datetime="";
String command="";
String parameter="";

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

void alarmChanged(uint8_t brightness) {
  if (brightness) {
    if (brightness == 255) {
      soundOk();
      Serial.println("Alarm on");
    }
  } else {
    soundNotOk();
    Serial.println("Alarm off");
  }
}

void computerChanged(uint8_t brightness) {
  int port=D1;
  if (brightness) {
    if (brightness == 255) {
      command="abort-shutdown";
      parameter="0";
      digitalWrite(port, HIGH);
      Serial.println("Computer ON");
    }
  } else {
    command="shutdown";
    parameter="3600";
    digitalWrite(port, LOW);
    Serial.println(port);
    Serial.println("Computer will whutdown in 1 hour");
  }
}

void waterChanged(uint8_t brightness) {
  
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
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int ledMax=30;

void rgb(int r, int g, int b) {
    analogWrite(D6, map(r,0,255,0,ledMax));
    analogWrite(D7, map(g,0,255,0,ledMax));
    analogWrite(D8, map(b,0,255,0,ledMax));
}

void bip(int frequency, int delay_){
  tone(D5, frequency);
  delay(delay_);
  noTone(D5);
}

void soundSetup() {
  bip(500,100);
  bip(1000,100);
  bip(1500,100);
}

void soundOk() {
  rgb(0,255,0);
  bip(1000,100);
  rgb(0,0,0);
  bip(1200,100);
  rgb(0,255,0);
  bip(1400,100);
  rgb(0,0,0);
  bip(1600,100);
  rgb(0,255,0);
  bip(1800,100);
  rgb(0,0,0);
}

void soundNotOk() {
  rgb(255,0,0);
  bip(1800,100);
  rgb(0,0,0);
  bip(1600,100);
  rgb(255,0,0);
  bip(1400,100);
  rgb(0,0,0);
  bip(1200,100);
  rgb(255,0,0);
  bip(1000,100);
  rgb(0,0,0);
}

void soundTask() {
  rgb(255,255,255);
  bip(1200,100);
  rgb(0,0,0);
  bip(1800,100);
  rgb(0,0,255);
  bip(1400,100);
  rgb(0,0,0);
  bip(1000,100);
  rgb(255,255,255);
  bip(1600,100);
  rgb(0,0,0);
}


void soundAmbulance() {
  bip(1000,400);
  bip(1800,400);
}

void soundAlarm() {
  bip(1000,100);
  bip(1800,100);
}


void setup() {

    Serial.begin(115200);
    Serial.println("Hello world");

    WiFiMulti.addAP(ssid, pass);

    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);

    //buzzer
    pinMode(D5, OUTPUT);
    noTone(D5);

    soundSetup();

    //RGB
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    rgb(255,255,0); //yellow
  
    digitalWrite(D0, LOW); //  Computer
    digitalWrite(D1, LOW); //  Alarm
    digitalWrite(D2, HIGH); // Water (Relay = HIGH)
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    //digitalWrite(D5, HIGH);
  
      // Initialise wifi connection
    wifiConnected = connectWifi();
  
    if (wifiConnected) {
        
        // Define devices
        espalexa.addDevice("alarm", alarmChanged);
        espalexa.addDevice("computer", computerChanged);
        espalexa.addDevice("water", waterChanged);
        
        espalexa.begin();

        soundOk();
  
    } else {
        //while (1) {
            Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
            delay(2500);
            soundNotOk();
        //}
    }
}



bool bool_water=false;
int count_water=0;

int count=0;
void loop() {
    espalexa.loop();

    //soundAlarm();

    //jsonn();

    if (wifiConnected) {
      rgb(0,255,0);
    } else {
      rgb(255,0,0);
    }
  

    if (count==100) { //em média a cada 10 segundos
      count=0;
      if (WiFiMulti.run() == WL_CONNECTED) {

        WiFiClient client;
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        if (http.begin(client, api)) {  // HTTP

          rgb(0,0,255);

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

          if (command!="") {
            nestedData["uuid"] = "1wvsD2RkFwaMZE8RVvtWSSpc0qhVSKkY";
            nestedData["com"] = command;
            nestedData["parameter"] = parameter;
            command="";
            parameter="";
          }

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

              if (String(status)=="success") {
                
                const char* com = doc["data"]["com"];
                const char* par = doc["data"]["parameter"];

                String str_com=String(com);

                if (str_com!="") {
                  soundTask();

                  if (str_com=="alarm") {

                  }
                }

                /*
                parameter = String(par);
                if (String(com)=="shutdown") {
                  parameter = "3600";
                } else if (String(com)=="abort-shutdown") {
                  parameter = "0";
                }

                command = String(com);
                */
                

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
            rgb(255,0,0);
          }
          http.end();
        } else {
          rgb(255,0,0);
          Serial.printf("[HTTP] Unable to connect\n");
        }
      }
    }
    count++;
    delay(100);
}