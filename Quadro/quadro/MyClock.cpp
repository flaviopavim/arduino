#include <Arduino.h>

#include "WiFiConnection.h"  // Inclui o cabeçalho da conexão WiFi
#include "API.h"
#include "LEDMatrix.h"
#include "Draw.h"
#include "MyClock.h"

//String actualColor = "#ffffff";
int count = 0;
unsigned long previousMillis_ = 0;
const long interval_ = 1000;

void loopClock() {
  unsigned long currentMillis_ = millis();
    if (previousMillis_ == 0 || currentMillis_ - previousMillis_ >= interval_) {
        previousMillis_ = currentMillis_;

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

        //count++;
    //Serial.println(count);
    if (count > 60*15) { // a cada quinze minutos busca a hora denovo
      count=0;
      //resetFalled();
      //if (!bool_get_hour) {
        setTime();
      //}
    }




    }
}
