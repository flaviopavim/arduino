//portas e ligações
int buzzer1 = 2;
int buzzer2 = 2;

//liga ou desliga o buzzer
bool boolBuzzer1 = false;
bool boolBuzzer2 = false;

//inicialização do Arduino
void setup() {

}

//variáveis do loop
unsigned long previousMillis = 0;
long interval = 20;

//magic ;)
void loop() {

  //busca em qual milissegundo está
  unsigned long currentMillis = millis();

  //se a contagem de milissegundos for igual ao intervalo
  if (currentMillis - previousMillis >= interval) {

    //seta o tempo atual na variável
    previousMillis = currentMillis;
    
    //se o boolBuzzer for true
    if (boolBuzzer1) {
      //liga o buzzer
      tone(buzzer1, 523);
    } else {
      //desligar
      noTone(buzzer1);
    }
    
    //se o boolBuzzer for true
    if (boolBuzzer2) {
      //liga o buzzer
      tone(buzzer2, 523);
    } else {
      //desligar
      noTone(buzzer2);
    }

    //adiciona o contrário de boolBuzzer
    //se for false, seta como true
    //se for true, seta como false
    boolBuzzer1=!boolBuzzer1;
    boolBuzzer2=!boolBuzzer2;

  }
}