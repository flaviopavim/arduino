//portas e ligações
int buzzer = 2;

//liga ou desliga o buzzer
bool boolBuzzer = false;

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
    if (boolBuzzer) {
      //liga o buzzer
      tone(buzzer, 523);
    } else {
      //desligar
      noTone(buzzer);
    }

    //adiciona o contrário de boolBuzzer
    //se for false, seta como true
    //se for true, seta como false
    boolBuzzer=!boolBuzzer;

  }
}