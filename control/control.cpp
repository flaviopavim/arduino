//controlador Bluetooth pra Arduino
//feito para dispositivos Alexia System ou compatíveis
String value = "";
bool b[100];

void setup() {
    Serial.begin(9600);
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                int v = value.toInt();
                Serial.println(v);
                digitalWrite(v, !digitalRead(v));
                value = "";
            } else {
                value += c;
            }
        }
    }
}