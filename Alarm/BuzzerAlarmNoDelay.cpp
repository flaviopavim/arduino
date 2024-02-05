const int buzzer = 2; //buzzer on arduino pin 2

unsigned long previousMillis = 0;
const long interval = 1;

void setup() {
  pinMode(buzzer, OUTPUT);
  delay(1000);
}

int frequency_min=1500;
int frequency_max=2000;

int frequency_sum=5;

int frequency=1000;
bool bool_updown=true;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //tone(buzzer, frequency);
    
    if (frequency<frequency_min) {
      frequency=frequency_min; 
      bool_updown=true;
    }
    if (frequency>frequency_max) {
      frequency=frequency_max;
      bool_updown=false; 
    }

    if (bool_updown) {
      frequency+=frequency_sum;
    } else {
      frequency-=frequency_sum;
    }

  }
}