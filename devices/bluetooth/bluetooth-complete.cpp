//controlador bluetooth pro drone

void funcao() {
    digitalWrite(2,HIGH);
}


String value = "";
bool b[100];
void loopBluetooth() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                if (value==="Hello World") {
                    //comando 1
                }
                value = "";
            } else {
                value += c;
            }
        }
    }
}



void setup() {
    Serial.begin(9600);
    //Serial.write("hello world");
}
void loop() {
    loopBluetooth();
}