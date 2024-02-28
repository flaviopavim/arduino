unsigned long previousMillis = 0;
const long interval = 100;
unsigned long count=0;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;


    //1 décimo de segundo


    if (count%5==0) {
        //meio segundo
    }
    if (count%10==0) {
        //1 segundo
    }
    if (count%300==0) {
        //30 segundos
    }
    if (count%600==0) {
        //1 minuto
    }
    if (count%(600*5)==0) {
        //5 minutos
    }


    count++;
  }
}