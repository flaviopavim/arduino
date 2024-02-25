#include <Servo.h>
Servo myservo;
int pos = 180;
int min_= 0;
int delay_=30;
int lastPos=0;
void setup() {
  myservo.attach(12);
}
void loop() {
  for (pos = min_; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(delay_);
  }
  for (pos = 180; pos >= min_; pos -= 1) {
    myservo.write(pos);
    delay(delay_);              
  }
}
