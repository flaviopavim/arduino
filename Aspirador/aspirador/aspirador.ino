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

void loop() {

  loopRC();

  int distance = getDistance();

  bool beat = beatSensor();
  bool front_ = floorFront();
  bool left_ = floorLeft();
  bool right_ = floorRight();

  
}

