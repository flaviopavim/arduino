#include <Servo.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(12, 10);
int distance;

Servo myservo; 

void setup() {
  Serial.begin(9600);
  myservo.attach(13);
}

unsigned long previousMillis = 0;
const long interval = 1;

long intervalRadar=10;
long countRadar=10;

bool boolAngle=false;
int angle=90;

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (intervalRadar==countRadar) {
      intervalRadar=0;

      if (boolAngle) {
        angle++;
      } else {
        angle--;
      }
      
      if (angle>=180) {
        boolAngle=false;
      }
      if (angle<=0) {
        boolAngle=true;
      }

      myservo.write(angle);

      if (angle==0 || angle==45 || angle==90 || angle==135 || angle==180) {
        distance = ultrasonic.read();
        Serial.print("Distance in ");
        String text="";
        if (angle==0) {
          text="Right";
        }
        if (angle==45) {
          text="Right Front";
        }
        if (angle==90) {
          text="Front";
        }
        if (angle==135) {
          text="Left Front";
        }
        if (angle==180) {
          text="Left";
        }
        Serial.println(text+" "+String(distance));
      }

    }
    intervalRadar++;

  }
}