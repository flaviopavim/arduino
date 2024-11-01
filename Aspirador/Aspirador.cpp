//Portas utilizadas
#define CH1 3
#define CH2 5
#define CH3 6

//Portas dos relês
#define RELAY1 A0
#define RELAY2 A1
#define RELAY3 A2
#define RELAY4 A3

//Lê os canais
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){

  //Inicia serial pra monitorar o controle
  Serial.begin(115200);

  //Inicia os pinos do controle
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);

  //Relês de controle dos motores
  pinMode(RELAY1,OUTPUT); //relê 1
  pinMode(RELAY2,OUTPUT); //relê 2
  pinMode(RELAY3,OUTPUT); //relê 3
  pinMode(RELAY4,OUTPUT); //relê 4

}

//Variáveis pra receber dados do controle
int ch1Value, ch2Value, ch3Value;

//parar o carrinho
void stop() {
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
}

//andar pra trás
void back() {
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, LOW);
}

//andar pra frente
void front() {
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, HIGH);
}

//andar pra esquerda
void left() {
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, HIGH);
}

//andar pra direita
void right() {
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, LOW);
}

void loop() {

  //Lê o valor dos canais do Rádio Controle
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  
  //Printa no monitor serial pra ver os valores
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.println(ch3Value);

  //abaixo foi usado o canal 3 e 2 do rádio controle
  if (ch3Value>80) {
    //o carrinho vai pra esquerda
    left();
  } else if (ch3Value<-80) {
    //o carrinho vai pra direita
    right();
  } else if (ch2Value<30 && ch2Value>-30) {
    //o carrinho fica parado
    stop();
  } else if (ch2Value>80) {
    //vai pra frente
    front();
  } else if (ch2Value<-80) {
    //vai pra trás
    back();
  }
  
}