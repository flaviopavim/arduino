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
  write("btc",0);

  String bitcoinString = String(bitcoin);
  char temp[7];  // Array para armazenar 3 caracteres + '\0'

  String firstThree = bitcoinString.substring(0, 6);
  firstThree.toCharArray(temp, 7);  // Converte a String para char*
  write(temp, 6);  // Escreve os primeiros 3 caracteres

  //String secondThree = bitcoinString.substring(3, 6);
  //secondThree.toCharArray(temp, 4);  // Converte a String para char*
  //write(temp, 12);  // Escreve os primeiros 3 caracteres


  show();
}