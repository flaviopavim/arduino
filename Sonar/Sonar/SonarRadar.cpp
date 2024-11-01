//Bibliotecas para o servo e o sensor ultrasônico
#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
#include <Servo.h> 

//pinos do sensor ultrasônico
const int echoPin = 7; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 6; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO

//pino do servo
int pin = A0;

//variável pra controlar o servo
Servo servo; 

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

  //sensor ultrasônico
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  servo.attach(pin); 
}

//variáveis para os canais
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

int distancia; //VARIÁVEL DO TIPO INTEIRO

void hcsr04(){
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
 }

void loopRadio() {
  //Lê os canais do rádio (mantive o ch1 porque ele precisa ler na ordem de canais)
  ch1Value = readChannel(CH1, -100, 100, 0); //pra frente, pra trás, controle de velocidade e parado
  ch2Value = readChannel(CH2, -100, 100, 0); //esquerda e direita
}

void loopControl() {
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


unsigned long previousMillis = 0;
const long interval = 100;

int angle=0; //ângulo inicial do servo
int max_angle=45; //ângulo máximo do servo
bool angle_reverse=true; //orientação do ângulo do serv0

void loopUltrasonic() {
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (angle_reverse) {
        angle++;
    } else {
        angle--;
    }

    //ângulos onde faz leituras de distância
    if (
        angle == -90 || 
        angle == -45 || 
        angle == -30 || 
        angle == -15 || 
        angle ==   0 || 
        angle ==  15 || 
        angle ==  30 || 
        angle ==  45 || 
        angle ==  90
    ) {
        hcsr04(); // faz a leitura da distância
        if (distancia<15) {
            //tem objeto à menos de 15 cm
        }
    }

    //abertura do giro de leitura
    if (angle>max_angle) {
        angle_reverse=false;
    }
    if (angle<-max_angle) {
        angle_reverse=true;
    }

    //gira o servo até o ângulo
    servo.write(angle);   
  }
}

void loop() {
  loopRadio();
  loopControl();
  loopUltrasonic();
}