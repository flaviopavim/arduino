#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nEscaneando dispositivos I2C...");
  
  byte count = 0;
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Dispositivo encontrado no endereço 0x");
      Serial.println(i, HEX);
      count++;
    }
  }
  if (count == 0) Serial.println("Nenhum dispositivo encontrado.");
}

void loop() {}

