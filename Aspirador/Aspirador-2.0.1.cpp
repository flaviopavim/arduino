/*
    Arduino Vacuum Cleaner Project

    Created with care by Flávio Pavim
    Video about the vacuum cleaner: https://youtube........ COMING SOON

    -------------------------------------------------------------------

    Radio Control Notes:
    To use Radio Control with Arduino, you need to use PWM ports.
    On Arduino Pro Mini 5V, PWM ports are 3, 5, 6, 9, 10, 11.

    Ports used in this project:

    - Radio Control:                     3 and 5
    - Motors (H-bridge relay):           A0, A1, A2, A3
    - Floor Sensors:                     A4 (left), A5 (front), A6 (right)
    - Collision Sensor:                  A7
    - RGB LED:                           6, 9, 10
    - Buzzer:                            2
    - Ultrasonic Sensor:                 7 (Trigger), 8 (Echo)
    - Servo Motor:                       4
    - Free Ports:                        11, 12, 13
*/

/******************************************************************************/

#include "Ultrasonic.h" //Incluir a biblioteca do Ultrasonic

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* 
    
     Port Definitions 

*/

// Ultrasonic Sensor
#define ECHO 8
#define TRIGGER 7

//Floor Sensors
#define LEFT  A4
#define FRONT A5
#define RIGHT A6

// Collision Sensor
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

// Time control
unsigned long lastActivityTime = 0; // Last activity
const unsigned long inactivityDelay = 30000; // 30 seconds in miliseconds

// Ultrasonic initialization
Ultrasonic ultrasonic(TRIGGER,ECHO);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

// Initialization
void setup() {

    Serial.begin(9600); //Inicializa o serial monitor

    // Ultrasonic
    pinMode(ECHO, INPUT);
    pinMode(TRIGGER, OUTPUT);

    // Floor sensor
    pinMode(LEFT,INPUT);
    pinMode(FRONT,INPUT);
    pinMode(RIGHT,INPUT);

    // Beat sensor
    pinMode(BEAT,INPUT);

    // Ultrasonic Servo
    pinMode(SERVO,INPUT);

    // H-Bridge Relays
    pinMode(A0,OUTPUT); //relê 1
    pinMode(A1,OUTPUT); //relê 2
    pinMode(A2,OUTPUT); //relê 3
    pinMode(A3,OUTPUT); //relê 4

    // pinMode(A5,OUTPUT); // Velocity relay

    // Radio control
    pinMode(CH1, INPUT); //1º canal do rádio controle
    pinMode(CH2, INPUT); //2º canal do rádio controle

    // RGB Led
    pinMode(R,OUTPUT);
    pinMode(G,OUTPUT);
    pinMode(B,OUTPUT);

    // Buzzer
    pinMode(BUZZER,OUTPUT);

}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Sensors and Inputs

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

void loopFloorDetector() {
    boolHoleFront=false;
    boolHoleLeft=false;
    boolHoleRight=false;
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

    Movement control

*/

void stop() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, HIGH);
}

void back() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, LOW);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, LOW);
}

void front() {
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE4, HIGH);
}

void left() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, LOW);
  digitalWrite(RELE3, LOW);
  digitalWrite(RELE4, HIGH);
}

void right() {
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, HIGH);
  digitalWrite(RELE3, HIGH);
  digitalWrite(RELE4, LOW);
}

// Automatic
// quando usar o rádio controle, ele seta como false
// se parar de usar por um tempo, seta como true
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

//reseta a contagem pra voltar ao automático
void resetAuto() {
    radio=true;
    automatic=false;
    automatic_counter=0;
}

void loopRC() {

    bool hasActivity=false;

    //Lê os canais do rádio
    ch1Value = readChannel(CH1, -100, 100, 0); //pra frente, pra trás e parado
    ch2Value = readChannel(CH2, -100, 100, 0); //esquerda e direita

    //abaixo foi usado o canal 3 e 2 do rádio controle
    if (ch2Value>80) {
      //o carrinho vai pra esquerda
      resetAuto();
      left();
      hasActivity=true;
    } else if (ch2Value<-80) {
      //o carrinho vai pra direita
      resetAuto();
      right();
      hasActivity=true;
    } else if (ch1Value<15 && ch1Value>-15) {
      //o carrinho fica parado
      resetAuto();
      stop();
      hasActivity=true;
    } else if (ch1Value>80) {
      //vai pra frente
      resetAuto();
      front();
      hasActivity=true;
    } else if (ch1Value<-80) {
      //vai pra trás
      resetAuto();
      back();
      hasActivity=true;
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

    return hasActivity;
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

void loopAutomaticByMe() {
    if (sensor_beat) {
        //se bater
        back();
        delay(500);
        choose();
    } else {
        //aqui não bateu ainda
        if (!sensor_front) {
            //se não tiver piso à frente
            choose();
        } else if (!sensor_front) {
            // Se não há piso à frente, tenta contornar
            choose();
        } else if (distance < 20) {
            // Se o obstáculo estiver muito próximo, tenta desviar
            if (sensor_left) {
                left();
            } else if (sensor_right) {
                right();
            } else {
                back();
            }
        } else {
            // Movimento padrão para frente se o caminho estiver limpo
            front();
        }
}

void loopAutomaticByGPT() {

    static unsigned long lastMoveTime = 0; // Armazena o tempo da última mudança de movimento
    const unsigned long moveDelay = 200;  // Tempo mínimo entre mudanças de movimento em milissegundos
    unsigned long currentTime = millis();

    if (currentTime - lastMoveTime < moveDelay) return; // Evita mudanças muito rápidas

    if (sensor_beat) {
        //se bater
        back();
        delay(500);
        choose();
    } else if (!sensor_front) {
        // Se não há piso à frente, escolhe outra direção
        choose();
    } else if (sensor_left && sensor_right) {
        // Ambas laterais livres, segue em frente
        front();
    } else if (sensor_left) {
        // Apenas a esquerda livre
        left();
    } else if (sensor_right) {
        // Apenas a direita livre
        right();
    } else {
        // Sem opções, recua
        back();
    }

    lastMoveTime = currentTime; // Atualiza o tempo da última mudança
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*

    Funcionamento em loop

*/

void loop() {

    // Captura o tempo atual
    unsigned long currentTime = millis();
    
    // Busca dados do controle e movimenta se houver
    if (loopRC()) { 
        lastActivityTime = currentTime; // Reinicia o tempo da última atividade
    }

    // lê os sensores
    loopUltrasonic();    // sensor de distância (ultrasonic)
    loopFloorDetector(); // sensor de piso

    // Verifica se houve 30 segundos de inatividade
    if (currentTime - lastActivityTime > inactivityDelay) {
        loopAutomaticByGPT();
        lastActivityTime = currentTime; // Reinicia o contador após executar a função
    }
}