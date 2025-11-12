#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Inicializando barômetro BMP085...");

  if (!bmp.begin()) {
    Serial.println("Erro: barômetro não detectado!");
    while (1);
  }

  Serial.println("BMP085 conectado com sucesso!");
}

void loop() {
  float temperatura = bmp.readTemperature();     // °C
  float pressao = bmp.readPressure();            // Pa
  float altitude = bmp.readAltitude(101325);     // m (nível do mar padrão)
  float altCorrigida = bmp.readAltitude(100800); // m (ajuste local)

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C  |  Pressão: ");
  Serial.print(pressao / 100.0);
  Serial.print(" hPa  |  Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  delay(500);
}

