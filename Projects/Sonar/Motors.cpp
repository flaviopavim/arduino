void setup(){
  Serial.begin(115200); //pra printar no monitor serial

  pinMode(A0,OUTPUT); //relê 1
  pinMode(A1,OUTPUT); //relê 2
  pinMode(A2,OUTPUT); //relê 3
  pinMode(A3,OUTPUT); //relê 4
}

//parar o carrinho
void stop() {
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
}

//andar pra esquerda
void front() {
  stop();
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

//andar pra trás
void back() {
  stop();
  digitalWrite(A0, LOW);
  digitalWrite(A3, LOW);
}

//andar pra frente
void left() {
  stop();
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
}

//andar pra direita
void right() {
  stop();
  digitalWrite(A1, LOW);
  digitalWrite(A3, LOW);
}


void loop() {
  back();
}