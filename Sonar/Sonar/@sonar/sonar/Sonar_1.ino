//controlador Bluetooth pra Arduino pro Mini
//feito para dispositivos Whitehats ou compatíveis
//https://whitehats.com.br/project-sonar
//

#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

String value = "";
bool b[100];
bool ia=true;
int buzzer=0;

void setup() {
    Serial.begin(9600);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A7, OUTPUT); //controlador de velocidade
    //seta estados iniciais dos relês
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
}

void stopSonar() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void front() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
}

void back() {
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
}

void left() {
  bool v=digitalRead(A7); //verifica a velocidade
  if (v) { //diminui a velocidade
    digitalWrite(A7,LOW);
  }
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  if (v) { //aumenta a velocidade
    digitalWrite(A7,HIGH);
  }
}

void right() {
  bool v=digitalRead(A7); //verifica a velocidade
  if (v) { //diminui a velocidade
    digitalWrite(A7,LOW);
  }
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
  if (v) { //aumenta a velocidade
    digitalWrite(A7,HIGH);
  }
}

void loop() {
  if (Serial.available() > 0) {
      char c = Serial.read();
      Serial.println(c);
      if (c) {
        if (c == '*') {
          value = "";
        } else if (c == '.') {
          if (value=="01") { //alterna velocidade
            digitalWrite(A7, !digitalRead(A7));
          } else if (value=="05") { //alterna ia
            ia=!ia;
          } else if (value=="03") { //pra frente
            front();
          } else if (value=="07") { //esquerda
            left();
          } else if (value=="08") { //parar
            stopSonar();
          } else if (value=="09") { //direita
            right();
          } else if (value=="13") { //ré
            back();
          }
          value = "";
      } else {
        value += c;
      }
    }
  }

  //leitura da distância dos objetos
  long microsec = ultrasonic.timing();
  float cm = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.println(cm);

  //pra não trombar nos objetos
  if (cm<10) { //objeto em menos de 10cm
    stopSonar();
  }

  if (ia) { //se a gamb.IA.rra estiver ligada
    //TODO: aqui é a mágica pra andar sozinho ;)
  }

  if (buzzer) {
    
  }
  
}
