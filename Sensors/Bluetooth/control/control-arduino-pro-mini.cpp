//controlador Bluetooth pra Arduino pro Mini
//feito para dispositivos Alexia System ou compatíveis
String value = "";
bool b[100];
void setup() {
    Serial.begin(9600);
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(A6, OUTPUT);
    pinMode(A7, OUTPUT);
}
void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                if (value=="2" || value=="3" || value=="4" || value=="5" || value=="6" || value=="7" || 
                    value=="8" || value=="9" || value=="10" || value=="11" || value=="12" || value=="13") {
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
                value = "";
            } else {
                value += c;
            }
        }
    }
}