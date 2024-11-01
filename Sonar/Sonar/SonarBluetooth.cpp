/*
    Controlador Bluetooth pra Arduino pro Mini
    Feito para dispositivos WhiteHats ou compatíveis
    Aplicativo pra controle:
        https://play.google.com/store/apps/details?id=br.com.whitehats.bluetooth
*/

void setup(){
  Serial.begin(115200); //pra printar no monitor serial

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

String value = "";
void loop() {
  if (Serial.available() > 0) {
      char c = Serial.read();
      Serial.println(c);
      if (c) {
          if (c=='s') {
              stop();
          } else if (c=='f') {
              front();
          } else if (c=='b') {
              back();
          } else if (c=='l') {
              left();
          } else if (c=='r') {
              right();
          } 
      }
  }
}