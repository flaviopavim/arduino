/*

    Aspirador de pó com Arduino Pro Mini

    Código feito com carinho por Flávio Pavim
    Vídeo sobre o aspirador: https://youtube........ EM BREVE

    -------------------------------------------------------------------

    Pra usar o Rádio Controle no Arduino, é necessário usar portas PWM
    No Arduino Pró Mini 5v as portas PWM's são 3,5,6,9,10,11

    Portas utilizadas nesse projeto:

        Rádio Controle:                  3 e 5
  
        Relês que controlam os motores  
        (ponte H com relê):              A0, A1, A2 e A3
  
        Sensor de piso da esquerda:      A4
        Sensor de piso da frente:        A5
        Sensor de piso da direita:       A6
  
        Sensor de batida:                A7
  
        Led RGB:                         6, 9 e 10
        Buzzer:                          2
  
        Ultrasonic: ????????             8 e 7
        Servo:      ????????             4
  
        Sobram as portas:                11, 12 e 13

*/

/******************************************************************************/

#include "Ultrasonic.h" //Incluir a biblioteca do Ultrasonic

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* 
    
    Portas 

*/

//Portas do sensor ultrasônico
//'TRIGGER' envia, 'ECHO' recebe
#define ECHO 8
#define TRIGGER 7

//sensores de piso
#define LEFT  A4
#define FRONT A5
#define RIGHT A6

//sensor de batida
#define BEAT  A7

#define SERVO 4

//relês - Ponte H
//pra definir movimentos do aspirador:
//  parado, frente, trás, esquerda, direita
#define RELE1 A0
#define RELE2 A1
#define RELE3 A2
#define RELE4 A3

//portas no Arduino que vai os canais do Rádio Controle
#define CH1 3
#define CH2 5

//portas dos leds
#define R 6
#define G 9
#define B 10

//buzzer
#define BUZZER 2

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Inicializa dispositivos e portas

*/

//Inicializa ultrasonic
Ultrasonic ultrasonic(TRIGGER,ECHO);

//Inicializa as portas
void setup() {

    Serial.begin(9600); //Inicializa o serial monitor

    //Ultrasonic
    pinMode(ECHO, INPUT); //pino de entrada
    pinMode(TRIGGER, OUTPUT); //pino de saída

    //Sensores de piso
    pinMode(LEFT,INPUT);
    pinMode(FRONT,INPUT);
    pinMode(RIGHT,INPUT);

    //Sensor de batida
    pinMode(BEAT,INPUT);

    //Servo do Ultrasonic
    pinMode(SERVO,INPUT);

    //Ponte H
    pinMode(A0,OUTPUT); //relê 1
    pinMode(A1,OUTPUT); //relê 2
    pinMode(A2,OUTPUT); //relê 3
    pinMode(A3,OUTPUT); //relê 4

    //pinMode(A5,OUTPUT); //relê de velocidade

    //Rádio Controle
    pinMode(CH1, INPUT); //1º canal do rádio controle
    pinMode(CH2, INPUT); //2º canal do rádio controle

    //Led RGB
    pinMode(R,OUTPUT);
    pinMode(G,OUTPUT);
    pinMode(B,OUTPUT);

    //Buzzer
    pinMode(BUZZER,OUTPUT);

}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Sensores e entradas

*/

int distance=0; //Distância do obstáculo no Ultrasonic

void loopUltrasonic(){
    //calcula a distância
    digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(TRIGGER, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(TRIGGER, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    distance = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    delay(500); //INTERVALO DE 500 MILISSEGUNDOS
}

//função pra ler os dados do Rádio Controle
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Atuadores e saídas

*/

//parar o aspirador
void stop() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, HIGH);
}

//andar pra trás
void back() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, LOW);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, LOW);
}

//andar pra frente
void front() {
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE4, HIGH);
}

//andar pra esquerda
void left() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, LOW);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE4, HIGH);
}

//andar pra direita
void right() {
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, LOW);
}

//modo automático
//quando usar o rádio controle, ele seta como false
//se parar de usar por um tempo, seta como true
bool automatic=true;
//contador pro modo automático
int automatic_counter=0;

//tem sinal de rádio?
bool radio=false; //se pegar sinal, seta como true

//interruptor de batida
bool sensor_beat=false; //false = sem batida, true = bateu

//sensores de piso
bool sensor_left=true; //true = tem piso, false = não tem piso
bool sensor_front=true; //true = tem piso, false = não tem piso
bool sensor_right=true; //true = tem piso, false = não tem piso

//variáveis dos canais do rádio
int ch1Value, ch2Value;

void resetAuto() {
    radio=true;
    automatic=false;
    automatic_counter=0;
}

void loopRC() {

    //Lê os canais do rádio
    ch1Value = readChannel(CH1, -100, 100, 0); //pra frente, pra trás e parado
    ch2Value = readChannel(CH2, -100, 100, 0); //esquerda e direita

    //abaixo foi usado o canal 3 e 2 do rádio controle
    if (ch2Value>80) {
      //o carrinho vai pra esquerda
      resetAuto();
      left();
    } else if (ch2Value<-80) {
      //o carrinho vai pra direita
      resetAuto();
      right();
    } else if (ch1Value<15 && ch1Value>-15) {
      //o carrinho fica parado
      resetAuto();
      stop();
    } else if (ch1Value>80) {
      //vai pra frente
      resetAuto();
      front();
    } else if (ch1Value<-80) {
      //vai pra trás
      resetAuto();
      back();
    }
   
    //Plus: controle da velocidade
    //if (ch1Value<-70 || ch1Value>70 || ch2Value<-70 || ch2Value>70) {
      //se empurrar o botão até o máximo,
      //o carrinho liga o relê que aciona voltagem maior pra ele acelerar
    //  digitalWrite(A5, LOW); 
    //} else {
      //quando o botão não está no máximo o carrinho anda mais devagar
    //  digitalWrite(A5, HIGH); 
    //}
    
    Serial.print(" CH1: ");
    Serial.print(ch1Value);
    Serial.print(" CH2: ");
    Serial.println(ch2Value);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Funções pro modo automático

*/

void choose() {
    //escolhe o lado que vai
    if (sensor_left && sensor_right) {
        //se as laterais estiverem livres
        //vai pra direita ou esquerda
        left();
    } else if (sensor_left) {
        //se a esquerda estiver livre
        //vai pra esquerda
        left();
    } else if (sensor_right) {
        //se a direita estiver livre
        //vai pra direita
        right();
    } else {
        //laterais não estão livres
        //vai pra trás
        back();
    }
}

void loopAutomatic() {
    if (sensor_beat) {
        //se bater
        choose();
    } else {
        //aqui não bateu ainda
        if (!sensor_front) {
            //se não tiver piso à frente
            choose();
        } else if (!sensor_left && !sensor_right) {
            //se não tiver piso nas laterais
            front();
        } else if (sensor_left) {
            //se a esquerda estiver livre
            //vai pra esquerda
            left();
        } else if (sensor_right) {
            //se a direita estiver livre
            //vai pra direita
            right();
        }
    }
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Funcionamento em loop

*/

void loop() {

    loopRC();

    automatic_counter++;
    if (automatic_counter>10) {
        automatic=true;
    }
    
    if (automatic) {
        loopAutomatic();
    }
}