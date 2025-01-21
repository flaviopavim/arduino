#include <Arduino.h>
#include "RC.h"
#include "Motors.h"

//Portas utilizadas
#define CH1 3
#define CH2 5
#define CH3 6

//Variáveis pra receber dados do controle
int ch1Value, ch2Value, ch3Value;

void setupRC() {
//Inicia os pinos do controle
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
}

//Lê os canais
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool loopRC() {

  //Lê o valor dos canais do Rádio Controle
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  
  //Printa no monitor serial pra ver os valores
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.println(ch3Value);

  //abaixo foi usado o canal 3 e 2 do rádio controle
  if (ch3Value>80) {
    //o carrinho vai pra esquerda
    left();
    return true;
  } else if (ch3Value<-80) {
    //o carrinho vai pra direita
    right();
    return true;
  } else if (ch2Value<30 && ch2Value>-30) {
    //o carrinho fica parado
    stop();
    return true;
  } else if (ch2Value>80) {
    //vai pra frente
    front();
    return true;
  } else if (ch2Value<-80) {
    //vai pra trás
    back();
    return true;
  }

  return false;
  
}
