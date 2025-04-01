#include <ArduinoJson.h>
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
#include "Matrix.h"

// Arduino setup function
void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);

    // Inicializa a conexão WiFi
    wifiConnected = connectWifi();
    delay(8);
    
    setupAlexa();
    //getAPI();
    setTime();
}

void loop() {
  loopAlexa();
  reset();
  effects();
  loopClock();
  write("bitcoin",0);

  String bitcoinString = bitcoin;
  char tempBitcoin[7];

  String firstThree = bitcoinString.substring(0, 6);
  firstThree.toCharArray(tempBitcoin, 7);  // Converte a String para char*
  write(tempBitcoin, 6);  // Escreve os primeiros 3 caracteres

  write("usd",12);
  String usdString = usd;
  char tempUSD[7];

  String secondThree = usdString.substring(0, 6);
  secondThree.toCharArray(tempUSD, 5);  // Converte a String para char*
  write(tempUSD, 18);  // Escreve os primeiros 3 caracteres

  show();
}