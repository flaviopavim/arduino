#include "Alexa.h"
#include "API.h"

Espalexa espalexa;

// Array de dispositivos com nome e status
struct Device {
    String name; // Nome do dispositivo
    int status;  // Status (0 = OFF, 1-255 = ON com brilho)
};

// Definição dos dispositivos
Device devices[] = {
    {"Quadro", 0}
};

// Função para ligar/desligar dispositivos pelo nome
void changeDeviceState(String deviceName, uint8_t brightness) {
    for (int i = 0; i < sizeof(devices) / sizeof(devices[0]); i++) {
        if (devices[i].name == deviceName) {
            devices[i].status = brightness; // Atualiza o status
            //Serial.print(devices[i].name + " changed to ");
            if (brightness) {
                Serial.println("Ligando computador");
            } else {
                Serial.println("Desligando computador");
            }
            return; // Sai da função após encontrar o dispositivo
        }
    }
    Serial.println("Device not found: " + deviceName);
}

//bool onOff=false;
bool pushButton=false;

// Callback para o dispositivo "Computer"
void computerChanged(uint8_t brightness) {
  
    //changeDeviceState("Computador", brightness);

    //if (brightness) {
      //if (!onOff) {
        //pushButton=true;
        //onOff=true;
      //}
      //sendCommand("21", "power-on");
    //} else {
      //if (onOff) {
        //pushButton=true;
        //onOff=false;
      //}
      //sendCommand("21", "shutdown");
    //}

    //if (pushButton) {
      //pushButton=false;
      //Serial.println("Push Button");
      // Controle do relé
      //digitalWrite(led, LOW);
      //digitalWrite(relay, LOW);
      //delay(300);
      //digitalWrite(led, HIGH);
      //digitalWrite(relay, HIGH);
    //}
}



void setupAlexa() {
    if (wifiConnected || connectWifi()) {
        // Define os dispositivos a partir do array
        espalexa.addDevice(devices[0].name, computerChanged);
        espalexa.begin(); // Inicia o Espalexa
    } else {
        while (1) {
            Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
            delay(2500);
        }
    }
}

void loopAlexa() {
    espalexa.loop();
    delay(1);
}
