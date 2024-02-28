#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
#include <Servo.h> 

const int echoPin = 7; //PINO DIGITAL UTILIZADO PELO HC-SR04 ECHO(RECEBE)
const int trigPin = 6; //PINO DIGITAL UTILIZADO PELO HC-SR04 TRIG(ENVIA)

Ultrasonic ultrasonic(trigPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO

int pin = A0;

Servo servo; 
 
unsigned long previousMillis = 0;
const long interval = 100;

void setup() {
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(trigPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  Serial.begin(9600); //INICIALIZA A PORTA SERIAL
  pinMode(ledPin, OUTPUT);
  servo.attach(pin); 
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

int angle=0;
bool up=true;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;


    if (up) {
        angle++;
    } else {
        angle--;
    }

    if (
        angle==-90 || 
        angle==-45 || 
        angle==0 || 
        angle==45 || 
        angle==90
    ) {
        hcsr04(); // FAZ A CHAMADA DO MÉTODO "hcsr04()"
        if (distancia<15) {
            //tem objeto à menos de 15 cm
        }
    }

    if (angle>90) {
        up=false;
    }
    if (angle<-90) {
        up=true;
    }

    servo.write(angle);   

    
  }
}