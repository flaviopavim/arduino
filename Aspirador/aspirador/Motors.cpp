#include <Arduino.h>
#include "Motors.h"
#include "RC.h"

#define RELAY1 A0
#define RELAY2 A1
#define RELAY3 A2
#define RELAY4 A3

void setupMotors() {
//Relês de controle dos motores
  pinMode(RELAY1,OUTPUT); //relê 1
  pinMode(RELAY2,OUTPUT); //relê 2
  pinMode(RELAY3,OUTPUT); //relê 3
  pinMode(RELAY4,OUTPUT); //relê 4
}

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
