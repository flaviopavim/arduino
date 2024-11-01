//controlador Bluetooth pra Arduino
//feito para dispositivos Alexia System ou compatíveis
String value = "";
bool b[100];
void setup() {
    Serial.begin(9600);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Serial.write("hello world");
}
void loop() {
  
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
          Serial.println(c);
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                if (value=="Red") {
                    digitalWrite(13, !digitalRead(13));
                }
                if (value=="Green") {
                    digitalWrite(12, !digitalRead(12));
                }
                if (value=="Blue") {
                    digitalWrite(11, !digitalRead(11));
                }
                value = "";
            } else {
                value += c;
            }
        }
    }
}