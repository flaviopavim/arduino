#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Espalexa.h>
#include <ArduinoJson.h>

// Cor atual para desenhar
String actualColor = "#ffffff";

#include "WiFiConnection.h"  // Inclui o cabeçalho da conexão WiFi
#include "API.h"
#include "LEDMatrix.h"
#include "Draw.h"

// Arduino setup function
void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    resetFalled();
    setTime();
}

int count = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

void loopClock() {
  unsigned long currentMillis = millis();
    if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        seconds++;
        if (seconds==60) {
          seconds=0;
          minutes++;
          if (minutes==60) {
            minutes=0;
            hours++;
            if (hours==24) {
              hours=0;
            }
          }
        }

        int x=0;

        if (bool_get_hour) {
          int hour1 = hours / 10;      // Primeiro dígito da hora
          int hour2 = hours % 10;      // Segundo dígito da hora

          int minute1 = minutes / 10;  // Primeiro dígito dos minutos
          int minute2 = minutes % 10;  // Segundo dígito dos minutos

          int second1 = seconds / 10;  // Primeiro dígito dos segundos
          int second2 = seconds % 10;  // Segundo dígito dos segundos

          // Exibindo os valores
          //Serial.println("");
          //Serial.print(hour1);
          //Serial.print(hour2);
          //Serial.print(":");
          //Serial.print(minute1);
          //Serial.print(minute2);
          //Serial.print(":");
          //Serial.print(second1);
          //Serial.print(second2);
          //Serial.println("");

          drawNumber(hour1,0+x);
          drawNumber(hour2,4+x);

          drawNumber(10,8+x);

          drawNumber(minute1,12+x);
          drawNumber(minute2,16+x);

          drawNumber(10,20+x);

          drawNumber(second1,24+x);
          drawNumber(second2,28+x);

        }

        if (seconds==0) {
          resetFalled();
        }

        count++;
        //Serial.println(count);
        if (count > 60*15) { // a cada quinze minutos busca a hora denovo
          count=0;
          //resetFalled();
          //if (!bool_get_hour) {
            setTime();
          //}
        }

        actualColor=randColor(); //muda a cor a cada 1 segundo
   
    }
}

void loop() {
    loopClock();
    all("#000000");
    fall();
    draw(); 
    FastLED.show();
}