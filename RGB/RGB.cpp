// LED 1
int R1 = 3;
int G1 = 5;
int B1 = 6;

// LED 2
int R2 = 9;
int G2 = 10;
int B2 = 11;

int brightness_r1 = 0;
int brightness_g1 = 0;
int brightness_b1 = 0;

int brightness_r2 = 0;
int brightness_g2 = 0;
int brightness_b2 = 0;

void apply() {
    
  analogWrite(R1, brightness_r1);
  analogWrite(G1, brightness_g1);
  analogWrite(B1, brightness_b1);

  analogWrite(R2, brightness_r2);
  analogWrite(G2, brightness_g2);
  analogWrite(B2, brightness_b2);
}

int fadeAmount = 5;

void setup() {

  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(B1, OUTPUT);

  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(B2, OUTPUT);
}

void loop() {

  apply();

  brightness_r1 = brightness_r1 + fadeAmount;
  if (brightness_r1 <= 0 || brightness_r1 >= 255) {
    fadeAmount = -fadeAmount;
  }

  brightness_g2 = brightness_g2 + fadeAmount;
  if (brightness_g2 <= 0 || brightness_g2 >= 255) {
    fadeAmount = -fadeAmount;
  }

  brightness_b2 = brightness_b2 + fadeAmount;
  if (brightness_b2 <= 0 || brightness_b2 >= 255) {
    fadeAmount = -fadeAmount;
  }

  brightness_r2 = brightness_r2 + fadeAmount;
  if (brightness_r2 <= 0 || brightness_r2 >= 255) {
    fadeAmount = -fadeAmount;
  }

  brightness_g2 = brightness_g2 + fadeAmount;
  if (brightness_g2 <= 0 || brightness_g2 >= 255) {
    fadeAmount = -fadeAmount;
  }

  brightness_b2 = brightness_b2 + fadeAmount;
  if (brightness_b2 <= 0 || brightness_b2 >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);

}