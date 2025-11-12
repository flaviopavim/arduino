/*

    Central de controle da casa com ESP32 + Alexa

    Alarm, garden control

    API UNDER CONSTRUCTION - COMING SOON

    D1 = Alarm
    D2 = Computer
    D3 = Water
    D4 = 
    D5 = Buzzer
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
//#ifdef ARDUINO_ARCH_ESP32
//#include <WiFi.h>
//#else
//#include <ESP8266WiFi.h>
//#endif
#include <Espalexa.h>

#include <ArduinoJson.h>

String api = "http://whitehats.com.br/api/whitehats/";
const char *ssid=""; // Nome do WiFi
const char *pass=""; // Senha do WiFi
const char *uuid="abc123";   // ID registrado na API


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define PAUSE 0

// change this to make the song slower or faster
int tempo=144; 

// change this to whichever pin you want to use
//int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, PAUSE, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, PAUSE, 4,
  
  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  PAUSE,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/2/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void tetris() {
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(D5, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(D5);
  }
}

bool bool_alarm=false;

// prototypes
boolean connectWifi();

//callback functions
void alarmChanged(uint8_t brightness);
void computerChanged(uint8_t brightness);
void waterChanged(uint8_t brightness);

ESP8266WiFiMulti WiFiMulti;

String datetime="";
String command="";
String parameter="";

boolean wifiConnected = false;

Espalexa espalexa;

void alarmChanged(uint8_t brightness) {
  if (brightness) {
    if (brightness == 255) {
      soundOk();
      Serial.println("Alarm on");
      bool_alarm=true;
    }
  } else {
    soundNotOk();
    Serial.println("Alarm off");
    bool_alarm=false;
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
      soundOk();
    }
  } else {
    command="shutdown";
    parameter="3600";
    digitalWrite(port, LOW);
    Serial.println("Computer will shutdown in 1 hour");
    soundNotOk();
  }
}

void waterChanged(uint8_t brightness) {
  int port = D2;
  if (brightness) {
    if (brightness == 255) {
      digitalWrite(port, HIGH);
      Serial.println("Water ON");
    }
  } else {
    digitalWrite(port, LOW);
    Serial.println("Water OFF");
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
  //bip(500,100);
  bip(1000,100);
  bip(1500,100);
}

void soundOk() {
  rgb(0,255,0);
  bip(500,100);
  rgb(0,0,0);
  bip(1000,100);
  rgb(0,255,0);
  bip(1500,100);
  rgb(0,0,0);
}

void soundNotOk() {
  rgb(255,0,0);
  bip(1500,100);
  rgb(0,0,0);
  bip(1000,100);
  rgb(255,0,0);
  bip(500,100);
  rgb(0,0,0);
}

void soundTask() {
  rgb(255,255,255);
  bip(NOTE_C5,200);
  rgb(0,0,0);
  bip(NOTE_D5,200);
  rgb(255,255,255);
  bip(NOTE_E5,200);
  rgb(0,0,0);
  bip(NOTE_F5,400);
  rgb(255,255,255);
  delay(25);
  rgb(0,0,0);
  bip(NOTE_F5,200);
  rgb(255,255,255);
  delay(25);
  rgb(0,0,0);
  bip(NOTE_F5,200);
  rgb(255,255,255);
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

    //pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);

    //buzzer
    pinMode(D5, OUTPUT);
    noTone(D5);

    soundSetup();
    tetris();

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

    if (bool_alarm) {
      rgb(255,255,0);
      delay(100);
    }

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
              datetime = String(doc["datetime"]);
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

              */

              Serial.println(datetime);

              String year  = datetime.substring(0,4);
              String month = datetime.substring(5,7);
              String day   = datetime.substring(8,10);
              String hour  = datetime.substring(11,13);
              String min   = datetime.substring(14,16);
              String sec   = datetime.substring(17,19);

              /*
              Serial.println(year);
              Serial.println(month);
              Serial.println(day);
              Serial.println(hour);
              Serial.println(min);
              Serial.println(sec);
              */

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
                    Serial.println("Water OFF");
                }

              }
              

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