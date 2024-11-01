/*
    Pra usar o Rádio Controle no Arduino, é necessário usar portas PWM
    No Arduino Pró Mini as portas PWM's são 3,5,6,9,10,11
*/

//portas no Arduino que vai os canais do Rádio Controle
#define CH1 3
#define CH2 5

//função pra ler os dados do Rádio Controle
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){
  Serial.begin(115200); //pra printar no monitor serial

  pinMode(CH1, INPUT); //1º canal do rádio controle
  pinMode(CH2, INPUT); //2º canal do rádio controle

  pinMode(A0,OUTPUT); //relê 1
  pinMode(A1,OUTPUT); //relê 2
  pinMode(A2,OUTPUT); //relê 3
  pinMode(A3,OUTPUT); //relê 4
  pinMode(A5,OUTPUT); //relê de velocidade
}

int ch1Value, ch2Value, ch3Value;

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
  //Lê os canais do rádio
  ch1Value = readChannel(CH1, -100, 100, 0); //pra frente, pra trás, controle de velocidade e parado
  ch2Value = readChannel(CH2, -100, 100, 0); //esquerda e direita

  //abaixo foi usado o canal 3 e 2 do rádio controle
  if (ch2Value>80) {
    //o carrinho vai pra esquerda
    left();
  } else if (ch2Value<-80) {
    //o carrinho vai pra direita
    right();
  } else if (ch1Value<15 && ch1Value>-15) {
    //o carrinho fica parado
    stop();
  } else if (ch1Value>80) {
    //vai pra frente
    front();
  } else if (ch1Value<-80) {
    //vai pra trás
    back();
  }

  //controle da velocidade
  if (ch1Value<-70 || ch1Value>70 || ch2Value<-70 || ch2Value>70) {
    //se empurrar o botão até o máximo,
    //o carrinho liga o relê que aciona voltagem maior pra ele acelerar
    digitalWrite(A5, LOW); 
  } else {
    //quando o botão não está no máximo o carrinho anda mais devagar
    digitalWrite(A5, HIGH); 
  }
  
  Serial.print(" CH1: ");
  Serial.print(ch1Value);
  Serial.print(" CH2: ");
  Serial.println(ch2Value);
}