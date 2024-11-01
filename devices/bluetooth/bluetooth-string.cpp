//controlador bluetooth pro drone
String value = "";
bool b[100];
void loopBluetooth() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                //cmd ex: 025
                String cmd = value.substring(0, 3);
                value = "";
            } else {
                value += c;
            }
        }
    }
}
