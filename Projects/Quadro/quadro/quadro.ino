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

// Variáveis de controle
unsigned long lastSwitch = 0;   // guarda o último tempo da troca
int screenState = 0;            // 0=bitcoin, 1=wallet, 2=hora
const unsigned long interval = 5000; // tempo entre trocas (5s)

unsigned long lastSwitchMinute = 0;   // guarda o último tempo da troca
const unsigned long intervalMinute = 60000; // tempo entre trocas (5s)

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2811, 2, RGB>(leds, NUM_LEDS);

    // Inicializa a conexão WiFi
    wifiConnected = connectWifi();
    delay(8);
    
    setupAlexa();
    setTime();
}

void loop() {
  //loopAlexa();
  
  reset();
  //effects();
  loopClock();
  
  if (millis() - lastSwitchMinute > intervalMinute) {
    lastSwitchMinute = millis();
    wifiConnected = connectWifi();
    delay(2);
    //setupAlexa();
    setTime();
  }

  // --- alternador de telas ---
  if (millis() - lastSwitch > interval) {
    lastSwitch = millis();
    screenState++;
    if (screenState > 2) screenState = 0;
  }

  // --- exibição ---
  switch (screenState) {
    case 0: { // BITCOIN
      write(" bitcoin",8);

      String bitcoinString = bitcoin;
      char tempBitcoin[7];
      String firstSix = bitcoinString.substring(0, 6);
      firstSix.toCharArray(tempBitcoin, 7);

      String text = "  " + String(tempBitcoin);
      String textColor = "#" + String(color);
      writeColor(text.c_str(), 14, textColor.c_str());
      break;
    }

    case 1: { // ETHEREUM
      write("ethereum",8);

      String ethereumString = ethereum;
      char tempEthereum[6];
      String firstSix = ethereumString.substring(0, 5);
      firstSix.toCharArray(tempEthereum, 6);

      String text = "   " + String(tempEthereum);
      //String textColor = "#" + String(color);
      write(text.c_str(), 14);
      break;
    }

    case 2: { // WALLET
      write("carteira", 8);

      String walletString = wallet;
      char tempWallet[10];
      String firstNine = walletString.substring(0, 9);
      firstNine.toCharArray(tempWallet, 10);

      String textWallet = "   " + String(tempWallet);
      write(textWallet.c_str(), 14);
      break;
    }

    case 3: {
      write("poha de", 1);
      write("api da", 7);
      write("pagar.me", 13);
      write("kkkkkkkk", 19);
      break;
    }

    case 4: {
      write("borajoga", 1);
      write("resident", 7);
      write("  evil 6", 13);
      write("   hasan", 19);
      break;
    }

    case 5: {
      write("   ozzy", 1);
      write("e melhor", 7);
      write("que iron", 13);
      write("  maiden", 19);
      break;
    }

    case 6: {
      write("   cuida", 1);
      write("      da", 7);
      write("     sua", 13);
      write("    vida", 19);
      break;
    }

    case 7: {
      String phraseString = phrase;
      char tempPhrase[7];
      String firstPhrase = phraseString.substring(0, 8);
      firstPhrase.toCharArray(tempPhrase, 7);
      String textPhrase = "  " + String(tempPhrase);
      write(textPhrase.c_str(), 12);
      break;
    }


  }

  show();
}


/*
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
  //loopAlexa();
  reset();
  //effects();
  loopClock();
  write(" bitcoin",0);

  String bitcoinString = bitcoin;
  char tempBitcoin[7];

  String firstThree = bitcoinString.substring(0, 6);
  firstThree.toCharArray(tempBitcoin, 7);  // Converte a String para char*

  String text = "  " + String(tempBitcoin);
  write(text.c_str(), 6);  // converte para const char*


  write("  wallet",12);

  String walletString = wallet;
  char tempWALLET[9];
  String secondThree = walletString.substring(0, 9);
  secondThree.toCharArray(tempWALLET, 9);  // Converte a String para char*

  String textWallet = "" + String(tempWALLET);
  write(textWallet.c_str(), 18);  // Escreve os primeiros 3 caracteres

  //write("usd",12);
  //String usdString = usd;
  //char tempUSD[7];

  //String secondThree = usdString.substring(0, 6);
  //secondThree.toCharArray(tempUSD, 5);  // Converte a String para char*
  //write(tempUSD, 18);  // Escreve os primeiros 3 caracteres

  show();
}
*/