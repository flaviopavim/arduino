/*
Controlador de portas arduino
*/
void setup() {
    Serial.begin(9600);
    for (int i = 0; i <= 100; i++) {
        pinMode(i, OUTPUT);
    }
}
String value = "";
bool b[100];
void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                //port ex: 025
                String p = value.substring(0, 3);
                if (!b[p.toInt()] || b[p.toInt()]==false) {
                    b[p.toInt()]=true;
                    analogWrite(p.toInt(), HIGH);
                } else {
                    b[p.toInt()]=false;
                    analogWrite(p.toInt(), LOW);
                }
                value = "";
            } else {
                value += c;
            }
        }
    }
}