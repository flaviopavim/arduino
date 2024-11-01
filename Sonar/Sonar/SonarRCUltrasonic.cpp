/*
    Código pra controle do carrinho Sonar

    O Sonar é controlado por Rádio Controle e com sistema automático

    Pra usar o Rádio Controle no Arduino, é necessário usar portas PWM
    No Arduino Pró Mini as portas PWM's são 3,5,6,9,10,11
*/

#include <Servo.h> 
#include "Ultrasonic.h"

int servoPin = 4;

const int echoPin = 8; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 7; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

//portas no Arduino que vai os canais do Rádio Controle
#define CH1 3
#define CH2 5

bool automatic=false;

Servo servo;  
Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO

int angle = 0;
bool invert_angle=false;


int distancia; //VARIÁVEL DO TIPO INTEIRO

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04(){
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    //delay(500); //INTERVALO DE 500 MILISSEGUNDOS
 }

//função pra ler os dados do Rádio Controle
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){
  Serial.begin(115200); //pra printar no monitor serial

  // Aciona o servo
  servo.attach(servoPin); 

  // Ultrasonic
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)

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


unsigned long previousMillis = 0;
const long interval = 1000;

int count=0;


void loopTimer() {
  unsigned long currentMillis = millis();
  //a cada 1 segundo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    count++;
    if (count>5) {
        count=0;
        automatic=true;
    }

    if (automatic) {

        // Movimento do servo
        if (!invert_angle) {
            angle++;
        } else {
            angle--;
        }

        if (angle>45) {
            invert_angle=true;
        }
        if (angle<-45) {
            invert_angle=false;
        }

        servo.write(angle);

        if (angle==0 || angle==-45 || angle==45) {

            hcsr04(); // faz leitura da distância

            if (distancia<10) {
                if (angle==0) {
                    // tem objeto à frente
                    stop();
                } else if (angle==-45) {
                    // tem objeto à esquerda
                    stop();
                } else if (angle==45) {
                    // tem objeto à direita
                    stop();
                }

                

            }

        }

        

    }

  }
}


void loopRC() {

  // Se usar o controle, desativa o automático
  automatic=false;

  // Lê os canais do rádio
  ch1Value = readChannel(CH1, -100, 100, 0); // pra frente, pra trás, controle de velocidade e parado
  ch2Value = readChannel(CH2, -100, 100, 0); // esquerda e direita

  // abaixo foi usado o canal 3 e 2 do rádio controle
  if (ch2Value>80) {
    // o carrinho vai pra esquerda
    left();
  } else if (ch2Value<-80) {
    // o carrinho vai pra direita
    right();
  } else if (ch1Value<15 && ch1Value>-15) {
    // o carrinho fica parado
    stop();
  } else if (ch1Value>80) {
    // vai pra frente
    front();
  } else if (ch1Value<-80) {
    // vai pra trás
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

void loop() {
  loopTimer();
  loopRC();
}