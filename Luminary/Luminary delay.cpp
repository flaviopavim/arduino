//portas pwm arduino pro mini
int lr[] = {3, 9}; //portas red dos leds
int lg[] = {5, 10}; //portas green dos leds
int lb[] = {6, 11}; //portas blue dos leds

void rgb(int r_, int g_, int b_, int l) {
    analogWrite(lr[l], r_);
    analogWrite(lg[l], g_);
    analogWrite(lb[l], b_);
}

void rgbs(int r_, int g_, int b_) {
    rgb(r_, g_, b_, 0);
    rgb(r_, g_, b_, 1);
}


void setup() {
    Serial.begin(9600);
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT);
        pinMode(lg[i], OUTPUT);
        pinMode(lb[i], OUTPUT);
    }
}

void blink_(int r_,int g_,int b_) {
  for (int i=1;i<=100;i++) {
    rgbs(r_, g_, b_);
    delay(20);
    rgbs(0, 0, 0);
    delay(20);
  }
}

void colors() {
  rgbs(0, 0, 40);
    delay(1000);
    rgbs(0, 40, 40);
    delay(1000);
    rgbs(0, 40, 0);
    delay(1000);
    rgbs(40, 40, 0);
    delay(1000);
    rgbs(40, 0, 0);
    delay(1000);
    rgbs(40, 0, 40);
    delay(1000);
}

int v=90;

void loop() {
    blink_(v,0,0);
    blink_(v,0,v);
    blink_(0,0,v);
    blink_(0,v,v);
    blink_(0,v,0);
    blink_(v,v,0);
    blink_(v,v,v);
}
