/*

    Central ESP32

    To garden control

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Alexa
//#ifdef ARDUINO_ARCH_ESP32
//#include <WiFi.h>
//#else
//#include <ESP8266WiFi.h>
//#endif
#include <Espalexa.h>

#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7

String api = "http://flaviopavim.com.br/esp/";
const char *ssid="";
const char *pass="";

// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);
void fifthLightChanged(uint8_t brightness);
void sixthLightChanged(uint8_t brightness);
void seventhLightChanged(uint8_t brightness);
void eigthLightChanged(uint8_t brightness);

ESP8266WiFiMulti WiFiMulti;

String datetime="";
String command="";

// device names
String Device_1_Name = "esp 1";
String Device_2_Name = "esp 2";
String Device_3_Name = "esp 3";
String Device_4_Name = "esp 4";
String Device_5_Name = "esp 5";
String Device_6_Name = "esp 6";
String Device_7_Name = "esp 7";
String Device_8_Name = "esp 8";

boolean wifiConnected = false;

Espalexa espalexa;

void lightChanged(port,brightness) {
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

void firstLightChanged(uint8_t brightness) {
  lightChanged(R1,brightness);
}

void secondLightChanged(uint8_t brightness) {
  lightChanged(R2,brightness);
}

void thirdLightChanged(uint8_t brightness) {
  lightChanged(R3,brightness);
}

void fourthLightChanged(uint8_t brightness) {
  lightChanged(R4,brightness);
}

void fifthLightChanged(uint8_t brightness) {
  lightChanged(R5,brightness);
}

void sixthLightChanged(uint8_t brightness) {
  lightChanged(R6,brightness);
}

void seventhLightChanged(uint8_t brightness) {
  lightChanged(R7,brightness);
}

void eigthLightChanged(uint8_t brightness) {
  if (brightness) {
    if (brightness == 255) {
        Serial.println("Alarm ON");
    }
  } else {
      Serial.println("Alarm OFF");
  }
}

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

void setup() {
    Serial.begin(9600);
    //WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, pass);

    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);
    pinMode(R5, OUTPUT);
    pinMode(R6, OUTPUT);
    pinMode(R7, OUTPUT);
  
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);
    digitalWrite(R5, HIGH);
    digitalWrite(R6, HIGH);
    digitalWrite(R7, HIGH);
  
  
      // Initialise wifi connection
    wifiConnected = connectWifi();
  
    if (wifiConnected) {
        
        // Define your devices here.
        espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
        espalexa.addDevice(Device_2_Name, secondLightChanged);
        espalexa.addDevice(Device_3_Name, thirdLightChanged);
        espalexa.addDevice(Device_4_Name, fourthLightChanged);
        espalexa.addDevice(Device_5_Name, fifthLightChanged);
        espalexa.addDevice(Device_6_Name, sixthLightChanged);
        espalexa.addDevice(Device_7_Name, seventhLightChanged);

        espalexa.addDevice(Device_8_Name, eigthLightChanged);
        
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
    if (count==100) { //em média a cada 10 segundos
      count=0;
      if (WiFiMulti.run() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        Serial.print("[HTTP] begin...\n");
        if (http.begin(client, api)) {  // HTTP
          Serial.print("[HTTP] GET...\n");
          int httpCode = http.GET();
          if (httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {

              String get=http.getString();

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

                if (count_water==0) {
                    //liga a água
                    digitalWrite(R2, LOW);
                    Serial.println("Device2 ON");
                }

                count_water++;

                //desliga a água em 2 minutos
                //a função é puxada a cada 10 segundos
                //60 segundos = 6 x 10 segundos
                //2 x 60 segundos = 2 minutos
                if (count_water==6*2) {
                    //reinicia o contador
                    count_water=0;
                    bool_water=false;
                    //desliga a água
                    digitalWrite(R2, HIGH);
                    Serial.println("Device2 OFF");
                }

              }

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