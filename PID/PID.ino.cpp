// Bibliotecas necessárias
#include <Wire.h> // Biblioteca para comunicação I2C
#include <Adafruit_Sensor.h> // Biblioteca para sensores Adafruit
#include <Adafruit_BNO055.h> // Biblioteca para o sensor BNO055

// Pinos utilizados para a comunicação I2C
#define SDA_PIN 4
#define SCL_PIN 5

// Objeto do sensor BNO055
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Constantes do controlador PID
double kp = 0.5; // Ganho proporcional
double ki = 0.2; // Ganho integral
double kd = 0.1; // Ganho derivativo

// Variáveis do controlador PID
double setpoint = 0; // Valor de referência
double erro = 0; // Erro
double integral = 0; // Integral acumulada
double derivativo = 0; // Derivativo
double last_time = 0; // Último tempo de leitura

// Função para calcular o controle PID
double calcularPID(double erro, double integral, double derivativo) {
  double output = kp * erro + ki * integral + kd * derivativo;
  return output;
}

void setup() {
  // Inicialização da comunicação serial
  Serial.begin(9600);

  // Inicialização do sensor BNO055
  if (!bno.begin()) {
    Serial.println("Não foi possível encontrar o sensor BNO055. Verifique a conexão!");
    while (1);
  }

  // Calibração do sensor BNO055
  delay(1000);
  bno.setExtCrystalUse(true);

  // Inicialização do último tempo de leitura
  last_time = millis();
}

void loop() {
  // Cálculo do tempo decorrido desde a última leitura
  double current_time = millis();
  double delta_time = (current_time - last_time) / 1000.0; // Delta de tempo em segundos
  last_time = current_time;

  // Leitura do valor do giroscópio
  sensors_event_t event;
  bno.getEvent(&event);
  double valor_giroscopio_x = event.gyro.x; // Valor do giroscópio na direção x

  // Cálculo do erro
  erro = setpoint - valor_giroscopio_x;

  // Cálculo do termo integral
  integral += erro * delta_time;

  // Cálculo do termo derivativo
  derivativo = (erro - last_erro) / delta_time;

  // Cálculo do controle PID
  double output = calcularPID(erro, integral, derivativo);

  // Aplicação do controle PID no quadricóptero
  // Substitua esta parte do código com as ações necessárias para o controle do quadricóptero com o valor de 'output' calculado pelo PID

  // Atualização do valor do erro para a próxima iteração
  last_erro = erro;
}