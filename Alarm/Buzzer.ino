const int buzzer = 4; //buzzer to arduino pin 4

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 4 as an output
}

void loop(){
  tone(buzzer, 1500); // Send 1KHz sound signal...
  delay(100);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(100);        // ...for 1sec
}
