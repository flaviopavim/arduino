// Portas e ligações
const int buzzer = 2;

// Inicialização do Arduino
void setup() {
  pinMode(buzzer, OUTPUT); // Inicializar o buzzer
}

// Variáveis do loop
unsigned long previousMillis = 0;
const long interval = 1;
int countInterval = 0;
const long intervalBuzzer = 10;

// Variáveis do filtro de média móvel
const int numReadings = 10; // Número de leituras a serem usadas na média móvel
int readings[numReadings];  // Array para armazenar as leituras
int readIndex = 0;          // Índice da próxima leitura
int total = 0;              // Total das leituras

// Função para calcular a média móvel
int getAverage() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(A1);
  total = total + readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  return total / numReadings;
}

// Magic ;)
void loop() {
  // Busca em qual milissegundo está
  unsigned long currentMillis = millis();

  // Filtrar o ruído do sensor com média móvel
  int filteredVoltage = getAverage() * (5.0 / 1023.0) * 100;

  // Mapear a leitura filtrada para a nota do buzzer
  int note = map(filteredVoltage, 0, 3300, 100, 10000);

  // Se a contagem de milissegundos for igual ao intervalo
  if (currentMillis - previousMillis >= interval) {
    // Seta o tempo atual na variável
    previousMillis = currentMillis;

    countInterval++;

    if (countInterval >= intervalBuzzer) {
      countInterval = 0;
      // Liga ou desliga o buzzer com base na nota
      tone(buzzer, note);
    }
  }
}

