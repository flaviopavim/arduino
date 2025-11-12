/*  
    Arduino with PIR motion sensor
*/
 
int led = 13;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

const int buzzer = 4;        //buzzer to arduino pin 4

void sound(int i) {
  tone(buzzer, i);
  delay(30);
  noTone(buzzer);
  delay(30);
}

void play(){
  sound(1000);
  sound(1200);
  sound(1400); 
  //delay(100);        // ...for 1sec
}

void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  Serial.begin(9600);        // initialize serial
}

void loop(){
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    //delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
      play();
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      //delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
}