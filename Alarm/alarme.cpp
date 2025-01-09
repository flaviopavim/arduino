//int ledPin = 13;
int speakerPin = 8;
int pinopir = 7;

// Criando as notas musicais (C,D,E,F,G,A,B e derivações ) em função da sua frequência
#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

void setup() {
  //pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(pinopir, INPUT);
}

bool alarm=false;
int count=0;

void loop() {
  int sensor=digitalRead(pinopir);
  if (sensor==1) {
    alarm=true;
  }
  if (alarme==true) {
    count++;
    if (count>=3) {
      count=0;
      alarm=false;
    }
    //beep(speakerPin, c, 70);
    //beep(speakerPin, d, 70);
    //beep(speakerPin, e, 70);
    //beep(speakerPin, f, 70);
    //beep(speakerPin, g, 70);
    //beep(speakerPin, a, 70);
    //beep(speakerPin, b, 70);
    //beep(speakerPin, cH, 70);


    beep(speakerPin, 1500, 70);
    beep(speakerPin, 2000, 70);

    /*
    for (int x=2000;x<3800;x++) {
      x+=10;
      beep(speakerPin, x, 1);
    }
    for (int x=3800;x>2000;x--) {
      x-=10;
      beep(speakerPin, x, 1);
    }
    */
  }
}

void beep(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds) {
    //digitalWrite(ledPin, HIGH);
    int x;
    long delayAmount = (long)(1000000/frequencyInHertz);
    long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
    for (x=0;x<loopTime;x++) {
        digitalWrite(speakerPin,HIGH);
        delayMicroseconds(delayAmount);
        digitalWrite(speakerPin,LOW);
        delayMicroseconds(delayAmount);
    }
    //digitalWrite(ledPin, LOW);
}