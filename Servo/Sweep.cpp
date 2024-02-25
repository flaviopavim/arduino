#include <Servo.h> 

int servoPin = 4;

Servo servo;  
int angle = 0;
 
void setup() { 
  servo.attach(servoPin); 
}
 
void loop() {
  for(angle = 40; angle < 130; angle++) {                                  
    servo.write(angle);               
    delay(15);                   
  }
  for(angle = 130; angle > 40; angle--) {                                
    servo.write(angle);           
    delay(15);       
  } 
} 