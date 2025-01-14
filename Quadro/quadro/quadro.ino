#include <FastLED.h>

#include <WiFiClient.h>
#include <Espalexa.h>

String actualColor = "#ffffff";

//#include "WiFiConnection.h"  // Inclui o cabeçalho da conexão WiFi
//#include "API.h"
#include "LEDMatrix.h"
//#include "Draw.h"
//#include "MyClock.h"

// Arduino setup function
void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    //resetFalled();
    //setTime();
}

/*

//efeito gangorra com relógio
int x = 0;
int y = 0;
bool bool_x = true;
bool bool_y = true;

void drawClock() {

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

      //efeito gangorra com relógio
      if (x>=2) {
        bool_x=false;
      } else if (x<=0) {
        bool_x=true;
      }

      if (y>=27) {
        bool_y=false;
      } else if (y<=0) {
        bool_y=true;
      }

      if (bool_x) {
        x++;
      } else {
        x--;
      }

      if (bool_y) {
        y++;
      } else {
        y--;
      }

      drawNumber(hour1,0+x,y);
      drawNumber(hour2,4+x,y);
      drawNumber(10,8+x,y);
      drawNumber(minute1,12+x,y);
      drawNumber(minute2,16+x,y);
      drawNumber(10,20+x,y);
      drawNumber(second1,24+x,y);
      drawNumber(second2,28+x,y);

    }

    if (seconds==0) {
      resetFalled();
    }

    actualColor=randColor(); //muda a cor a cada 1 segundo
}
*/

//unsigned long previousMillis = 0;
//const long interval = 1000;
int x=0;
int y=0;
bool bool_x=true;
bool bool_y=true;
void loop() {

  //loopClock();

  //unsigned long currentMillis = millis();
  //if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
  //    previousMillis = currentMillis;
      // a cada 1 segundo
      //all("#000000");
      //drawClock();
      //draw(); 
  //}

  //for (int i = 0; i < 50; i++) {
  //      pixel(random(1, 33), random(1, 33), randColor());
  //  }
  all("#000000");

  if (x>=33) {
    bool_x=false;
  } else if (x<=0) {
    bool_x=true;
  }

  if (y>=33) {
    bool_y=false;
  } else if (y<=0) {
    bool_y=true;
  }

  if (bool_x) {
    x++;
  } else {
    x--;
  }

  if (bool_y) {
    y++;
  } else {
    y--;
  }
  
  pixel(x,y, randColor());
  //fall();
  FastLED.show();
  delay(10);
}