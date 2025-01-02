const int buzzer = 2; //buzzer to arduino pin 2

void bip(int frequency, int delay_){
  tone(buzzer, frequency);
  delay(delay_);
  noTone(buzzer);
}

void soundSetup() {
  bip(500,100);
  bip(1000,100);
  bip(1500,100);
}

void soundAmbulance() {
  bip(1000,400);
  bip(1800,400);
}

void soundAlarm() {
  bip(1000,100);
  bip(1800,100);
}

void alarmAll() {
  for(int i=1;i<=32;i++) {
    soundAlarm();
  }
  for(int i=1;i<=8;i++) {
    soundAmbulance();
  }
}

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 4 as an output
  soundSetup();
  delay(1000);
}

void loop() {
   alarmAll();
}