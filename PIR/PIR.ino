// Define os pinos para o sensor PIR e para o LED
const int pinoPIR = 2;   // Pino digital para o sensor PIR (OUT)
const int pinoLED = 13;  // Pino digital para o LED (integrado ou externo)

// Variável para armazenar o estado de leitura do sensor
int estadoPIR = 0;

void setup() {
  // Configura o pino do LED como saída
  pinMode(pinoLED, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  // Configura o pino do sensor PIR como entrada
  pinMode(pinoPIR, INPUT);
  
  // Inicia a comunicação serial para monitorar o estado
  Serial.begin(9600);
  
  Serial.println("Calibrando o sensor PIR...");
  // Aguarda alguns segundos para o sensor se estabilizar
  delay(15000); 
  Serial.println("Sensor PIR pronto!");
}

void loop() {
  // Lê o estado do pino do sensor (HIGH se houver movimento, LOW se não)
  estadoPIR = digitalRead(pinoPIR);
  
  // Verifica se o estado é HIGH (movimento detectado)
  if (estadoPIR == HIGH) {
    // Acende o LED
    digitalWrite(pinoLED, HIGH);
    analogWrite(3, HIGH);
    analogWrite(5, HIGH);
    analogWrite(6, HIGH);
    Serial.println("Movimento detectado!");
  } else {
    // Apaga o LED
    digitalWrite(pinoLED, LOW);
    analogWrite(3, LOW);
    analogWrite(5, LOW);
    analogWrite(6, LOW);
    Serial.println("Nenhum movimento");
  }
}
