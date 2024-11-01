//arduino pro mini pwm pins
int lr[] = {3, 9}; //red pins
int lg[] = {5, 10}; //green pins
int lb[] = {6, 11}; //blue pins
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
                //red,green,blue,led
                //025,220,125,0
                //025,220,125,1
                String r = value.substring(0, 3); //red (0~255)
                String g = value.substring(4, 7); //green (0~255)
                String b = value.substring(8, 11); //blue (0~255)
                String l = value.substring(12, 13); //led (0 or 1)
                rgb(l.toInt(), r.toInt(), g.toInt(), b.toInt()); //led e as intensidades de cores r,g,b
                value = "";
            } else {
                value += c;
            }
        }
    }
}