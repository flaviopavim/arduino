#include <Servo.h> 

// Postas do Arduino
//-----------------------------------------
int pin_servo_1 = A0;
int pin_servo_2 = A1;

#define CH1 3 
#define CH2 5
#define CH3 6
//-----------------------------------------

Servo servo_1, servo_2; 

const int numReadings = 2;
int ch1Readings[numReadings];
int ch2Readings[numReadings];
int ch3Readings[numReadings];
int ch1Index = 0;
int ch2Index = 0;
int ch3Index = 0;
int ch1Total = 0;
int ch2Total = 0;
int ch3Total = 0;

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){
  Serial.begin(115200);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2);

  // Inicializa os arrays de leituras com valores padrão
  for (int i = 0; i < numReadings; i++) {
    ch1Readings[i] = 0;
    ch2Readings[i] = 0;
    ch3Readings[i] = 0;
  }
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int angle_servo_1 = 80;
int angle_servo_2 = 100;

void updateReading(int& index, int& total, int* readings, int newValue) {
  // Subtrai o valor antigo da média
  total -= readings[index];
  // Adiciona o novo valor à média
  readings[index] = newValue;
  total += newValue;
  // Incrementa o índice e reinicia se necessário
  index++;
  if (index >= numReadings) {
    index = 0;
  }
}

int getFilteredReading(int total, int* readings) {
  // Calcula a média dos valores
  return total / numReadings;
}

void loop() {
  // Faz leitura do Rádio Controle
  int ch1Value = readChannel(CH1, -100, 100, -100);
  int ch2Value = readChannel(CH2, -100, 100, 0);
  int ch3Value = readChannel(CH3, -100, 100, 0);
  
  // Atualiza os arrays de leituras
  updateReading(ch1Index, ch1Total, ch1Readings, ch1Value);
  updateReading(ch2Index, ch2Total, ch2Readings, ch2Value);
  updateReading(ch3Index, ch3Total, ch3Readings, ch3Value);

  // Obtém os valores filtrados
  int filteredCh1Value = getFilteredReading(ch1Total, ch1Readings);
  int filteredCh2Value = getFilteredReading(ch2Total, ch2Readings);
  int filteredCh3Value = getFilteredReading(ch3Total, ch3Readings);
  
  // Printa os valores no serial monitor
  Serial.print(" CH2: ");
  Serial.print(filteredCh2Value);
  Serial.print(" CH3: ");
  Serial.println(filteredCh3Value);

  // Mapeia o valor do servo proporcional ao que vem do rádio controle
  double aileron = map(filteredCh2Value, -120, 120, -45, 45);
  double elevator = map(filteredCh3Value, -120, 120, -45, 45);

  // Move os servos
  servo_1.write(angle_servo_1 + elevator - aileron);   
  servo_2.write(angle_servo_2 - elevator - aileron);
}
