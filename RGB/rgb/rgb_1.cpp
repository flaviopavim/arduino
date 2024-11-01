//portas pwm arduino pro mini
int lr[] = {3, 9}; //portas red dos leds
int lg[] = {5, 10}; //portas green dos leds
int lb[] = {6, 11}; //portas blue dos leds

void rgb(int l, int r, int g, int b) {
    analogWrite(lr[l], r);
    analogWrite(lg[l], g);
    analogWrite(lb[l], b);
}

//função extra
//String hex(String hexstring) {
////    String hexstring = "B787B7";
//    long number = (long) strtol( &hexstring[1], NULL, 16);
//    int r = number >> 16;
//    int g = number >> 8 & 0xFF;
//    int b = number & 0xFF;
//    return String(r)+","+String(g)+","+String(b);
//}

void setup() {
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT);
        pinMode(lg[i], OUTPUT);
        pinMode(lb[i], OUTPUT);
    }
}

void loop() {
    rgb(0, 255, 100, 150); //led 1 e as intensidades de cores r,g,b
    rgb(1, 255, 100, 150); //led 2 e as intensidades de cores r,g,b
}