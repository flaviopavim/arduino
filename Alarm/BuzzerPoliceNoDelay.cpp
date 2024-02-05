const int buzzer = 2; //buzzer to arduino pin 2

unsigned long previousMillis = 0;
const long interval = 1;

void setup() {
  pinMode(buzzer, OUTPUT);
  delay(1000);
}

int frequency=1000;
bool bool_updown=true;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    tone(buzzer, frequency);
    
    if (frequency<1000) {
      frequency=1000; 
      bool_updown=true;
    }
    if (frequency>2000) {
      frequency=2000;
      bool_updown=false; 
    }

    if (bool_updown) {
      frequency++;
    } else {
      frequency--;
    }

  }
}
