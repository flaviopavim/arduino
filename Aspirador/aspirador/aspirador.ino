#include "Motors.h"
#include "RC.h"
#include "Distance.h"

void setup() {
  Serial.begin(115200);
  setupRC();
  setupMotors();
}

void loop() {
  getDistance();
  loopRC();
}

