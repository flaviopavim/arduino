#include "esp.h"
#include "api.h"
#include "led.h"

void setup() {
    Serial.begin(9600);
    connectWifi(); // Conecta ao WiFi
}

void loop() {
    // Código principal
    fall();       // Animação
    FastLED.show();
    delay(100);
}
