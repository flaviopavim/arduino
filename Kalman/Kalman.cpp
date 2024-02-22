#include <Wire.h> // Biblioteca para comunicação I2C
#include <Adafruit_Sensor.h> // Biblioteca para sensores Adafruit
#include <Adafruit_BME280.h> // Biblioteca para o sensor BME280
#include <KalmanFilter.h> // Biblioteca para o filtro de Kalman

// Definição dos pinos SDA e SCL para a comunicação I2C
#define SDA_PIN 4
#define SCL_PIN 5

// Objeto do sensor BME280
Adafruit_BME280 bme;

// Objeto do filtro de Kalman
KalmanFilter kalmanFilter;

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(9600);

  // Inicialização do sensor BME280
  if (!bme.begin(0x76)) {
    Serial.println("Não foi possível encontrar o sensor BME280. Verifique a conexão!");
    while (1);
  }

  // Inicialização do filtro de Kalman
  kalmanFilter.init(0, 1, 0, 1, 0.01); // Substitua os parâmetros de inicialização de acordo com sua aplicação
}

void loop() {
  // Leitura do valor do sensor
  float valorSensor = bme.readTemperature(); // Substitua por outra leitura de sensor, se necessário

  // Aplica o filtro de Kalman no valor do sensor
  float valorFiltrado = kalmanFilter.updateEstimate(valorSensor);

  // Imprime os valores originais e filtrados no monitor serial
  Serial.print("Valor original do sensor: ");
  Serial.println(valorSensor);
  Serial.print("Valor filtrado do sensor: ");
  Serial.println(valorFiltrado);
  Serial.println("-----------------------------");

  // Aguarda um curto intervalo de tempo entre as leituras
  delay(1000);
}