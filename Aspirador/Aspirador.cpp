#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

const int echoPin = 8; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 7; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO

int distancia; //VARIÁVEL DO TIPO INTEIRO
String result; //VARIÁVEL DO TIPO STRING

//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA
void hcsr04(){
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(trigPin, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(trigPin, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
    //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
    //NA VARIAVEL "distancia"
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    result = String(distancia); //VARIÁVEL GLOBAL DO TIPO STRING RECEBE A DISTÂNCIA(CONVERTIDO DE INTEIRO PARA STRING)
    delay(500); //INTERVALO DE 500 MILISSEGUNDOS
 }

/***************************************/

int relay1=A0;
int relay2=A1;
int relay3=A2;
int relay4=A3;

/***************************************/

void stop() {
    digitalWrite(relay1,HIGH);
    digitalWrite(relay2,HIGH);
    digitalWrite(relay3,HIGH);
    digitalWrite(relay4,HIGH);
}
void front() {
    stop();
}
void back() {
    stop();
}
void left() {
    stop();
}
void right() {
    stop();
}

/***************************************/

int getDistance() {
    hcsr04(); // FAZ A CHAMADA DO MÉTODO "hcsr04()"
    Serial.print("Distancia "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(result); ////IMPRIME NO MONITOR SERIAL A DISTÂNCIA MEDIDA
    Serial.println("cm"); //IMPRIME O TEXTO NO MONITOR SERIAL
    return result;
}

/***************************************/

void setup() {

    Serial.begin(9600); //INICIALIZA A PORTA SERIAL

    //inicia o Ultrasonic
    pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
    pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)

}

void loop() {
    int distance=getDistance();
}



