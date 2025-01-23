#include <FastLED.h>
#include <WiFiClient.h>
#include <Espalexa.h>

#include "WiFiConnection.h"
#include "Alexa.h"
#include "API.h"

#include "LED.h"
#include "Draw.h"
#include "MyClock.h"
#include "Effects.h"

// Arduino setup function
void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);

    // Inicializa a conexão WiFi
    wifiConnected = connectWifi();
    delay(5);
    
    setupAlexa();
    //getAPI();
    setTime();
}

void loop() {
  reset();
  effects();
  loopClock();
  show();
  loopAlexa();
}