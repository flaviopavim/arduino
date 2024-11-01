void setup(){
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

void loop() {
    front();
    stop();
}