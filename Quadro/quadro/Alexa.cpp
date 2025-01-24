#include "Alexa.h"

Espalexa espalexa;
String deviceNames[] = {"Computador", "Quadro"}; // Array com os nomes dos dispositivos

// Funções de callback para os dispositivos
void computerChanged(uint8_t brightness) {
    Serial.print(deviceNames[0] + " changed to ");
    if (brightness) {
        Serial.print("ON, brightness ");
        Serial.println(brightness);
    } else {
        Serial.println("OFF");
    }
}

void quadroChanged(uint8_t brightness) {
    Serial.print(deviceNames[1] + " changed to ");
    if (brightness) {
        Serial.print("ON, brightness ");
        Serial.println(brightness);
    } else {
        Serial.println("OFF");
    }
}

void setupAlexa() {
    if (wifiConnected || connectWifi()) {
        // Define os dispositivos usando os nomes do array
        espalexa.addDevice(deviceNames[0], computerChanged);
        espalexa.addDevice(deviceNames[1], quadroChanged);
        espalexa.begin(); // Inicia o Espalexa
        Serial.println("Dispositivos iniciados");
    } else {
        while (1) {
            Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
            delay(2500);
        }
    }
}

void loopAlexa() {
  Serial.println("Escutando comandos Alexa");
  espalexa.loop();
  delay(1);
}

