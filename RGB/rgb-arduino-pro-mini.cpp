//portas pwm arduino pro mini
int lr[] = {3, 9}; //portas red dos leds
int lg[] = {5, 10}; //portas green dos leds
int lb[] = {6, 11}; //portas blue dos leds
void rgb(int l, int r, int g, int b) {
    analogWrite(lr[l], r);
    analogWrite(lg[l], g);
    analogWrite(lb[l], b);
}
void setup() {
    Serial.begin(9600);
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT);
        pinMode(lg[i], OUTPUT);
        pinMode(lb[i], OUTPUT);
    }
}
String value = "";
void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c) {
            if (c == ':') {
                value = "";
            } else if (c == '.') {
                //r,g,b,led
                //025,220,125,0
                //025,220,125,1
                String r = value.substring(0, 3); //r
                String g = value.substring(4, 7); //g
                String b = value.substring(8, 11); //b
                String l = value.substring(12, 13); //l
                rgb(l.toInt(), r.toInt(), g.toInt(), b.toInt()); //led e as intensidades de cores r,g,b
                Serial.print(r);
                Serial.print(" - ");
                Serial.print(g);
                Serial.print(" - ");
                Serial.print(b);
                Serial.print(" - ");
                Serial.print(l);
                Serial.println("");
                value = "";
            } else {
                value += c;
            }
        }
    }
}