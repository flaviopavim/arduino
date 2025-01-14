#include <Arduino.h>
#include "WifiManager.h"
#include "LEDMatrix.h"
#include "Effects.h"

void setup() {
    Serial.begin(115200);
    connectWifi();
    FastLED.addLeds<WS2812, D2, GRB>(leds, NUM_LEDS);
}

void loop() {
    randPixels();
    fall();
    FastLED.show();
    delay(50);
}
