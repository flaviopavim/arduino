#include "Motors.h"
#include "RC.h"
#include "Distance.h"
#include "BeatSensor.h"
#include "FloorSensor.h"

void setup() {
  Serial.begin(115200);
  setupRC();
  setupMotors();
}

bool automatic = false;

unsigned long previousMillis = 0;
const long interval = 1000; //1 second
int seconds_to_automatic=0;

void loop() {

  // RC is controlling?
  bool rc = loopRC();

  // if interacts with RC
  if (rc) {
    seconds_to_automatic=0;
    automatic=false;
  }

  // each 1 second
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds_to_automatic++;
    if (seconds_to_automatic>=30) { //after 30 seconds
      automatic=true;
    }
  }

  int distance = getDistance();

  bool beat = beatSensor();
  bool front_ = floorFront();
  bool left_ = floorLeft();
  bool right_ = floorRight();

  if (automatic) {

    if (distance < 10) {

    }

    if (beat) {
      back();
      delay(100);
      right();
      delay(100);
      front();
    }

    if (front_) {

    }

  }

}

