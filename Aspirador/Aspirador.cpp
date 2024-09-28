#define CH1 3
#define CH2 5
#define CH3 6

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
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

  //Relês de controle dos motores
  pinMode(A0,OUTPUT); //relê 1
  pinMode(A1,OUTPUT); //relê 2
  pinMode(A2,OUTPUT); //relê 3
  pinMode(A3,OUTPUT); //relê 4

}

int ch1Value, ch2Value, ch3Value, ch4Value;
bool ch5Value;

//parar o carrinho
void stop() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
}

//andar pra trás
void back() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
}

//andar pra frente
void front() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
}

//andar pra esquerda
void left() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
}

//andar pra direita
void right() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
}

void loop() {

  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  
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
