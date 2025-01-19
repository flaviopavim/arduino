#include <FastLED.h>
#include <WiFiClient.h>
#include <Espalexa.h>

#include "WiFiConnection.h"
#include "API.h"
#include "LEDMatrix.h"
#include "Matrix.h"
#include "Draw.h"
#include "MyClock.h"
#include "Effects.h"

// Arduino setup function
void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);
    //resetFalled();
    setTime();
}

unsigned long previousMillis = 0;
const long interval = 1000;

void reset() {
  all("#000000");
}

void show() {
  FastLED.setBrightness(10);
  FastLED.show();
}

void loop() {

  reset();
  
  fireworks();

  unsigned long currentMillis = millis();
  if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // each 1 second
  }

  for (int i = 0; i < 5; i++) {
    pixel(random(1, 32), random(1, 32), randColor());
  }
  
  //fall();
  //pingPong();

  loopClock();
  drawClock();
  draw();

  show();

}