/*
    Controlador Bluetooth pra Arduino pro Mini
    Feito para dispositivos WhiteHats ou compatíveis
    Aplicativo pra controle:
        https://play.google.com/store/apps/details?id=br.com.whitehats.bluetooth
*/

String value = "";
void setup() {

    Serial.begin(9600);

    // liga todas as portas
    for (int i = 2; i <= 13; i++) {
        // seta as portas como porta de saída
        pinMode(i, OUTPUT);
        // seta como high pra desligar os relês
        digitalWrite(i, HIGH);
    }

    // Abaixo usa as portas Analógicas como portas digitais

    // seta as portas como porta de saída
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(A6, OUTPUT);
    pinMode(A7, OUTPUT);

    // seta as portas como high pra desligar os relês
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);
    digitalWrite(A6, HIGH);
    digitalWrite(A7, HIGH);

}
void loop() {
    // se o Bluetooth tiver ativo
    if (Serial.available() > 0) {
        // lê cada char que o Bluetooth envia
        char c = Serial.read();
        // printa cada char que passa no Bluetooth
        Serial.println(c);
        if (c) {
            // Exemplos de strings pra receber: *12., *13., *A1.
            if (c == '*') {
                // aqui é o início da string, limpa value
                value = "";
            } else if (c == '.') {
                // quando receber . é o fim da instrução. compara value pra saber qual porta ligar ou desligar
                if (value=="2" || value=="3" || value=="4" || value=="5" || value=="6" || value=="7" || 
                    value=="8" || value=="9" || value=="10" || value=="11" || value=="12" || value=="13") {
                    // liga ou desliga as portas digitais
                    int v = value.toInt();
                    digitalWrite(v, !digitalRead(v));
                } else if (value=="A0") {
                    digitalWrite(A0, !digitalRead(A0));
                } else if (value=="A1") {
                    digitalWrite(A1, !digitalRead(A1));
                } else if (value=="A2") {
                    digitalWrite(A2, !digitalRead(A2));
                } else if (value=="A3") {
                    digitalWrite(A3, !digitalRead(A3));
                } else if (value=="A4") {
                    digitalWrite(A4, !digitalRead(A4));
                } else if (value=="A5") {
                    digitalWrite(A5, !digitalRead(A5));
                } else if (value=="A6") {
                    digitalWrite(A6, !digitalRead(A6));
                } else if (value=="A7") {
                    digitalWrite(A7, !digitalRead(A7));
                } 
                // limpa a variável pra escrever a próxima instrução
                value = "";
            } else {
                // não é o início nem o fim, concatena o char na string (junta a letra na string)
                value += c;
            }
        }
    }
}