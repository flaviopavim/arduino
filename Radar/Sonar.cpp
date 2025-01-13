#include <Servo.h>
#include <Ultrasonic.h>

// Define relay pins
#define relay1 A3
#define relay2 A2
#define relay3 A1
#define relay4 A0

Servo servo; // Create servo object
Ultrasonic ultrasonic(12, 10); // Initialize ultrasonic sensor

// Function to stop all movement
void stopMovement() {
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}

void setup() {
  Serial.begin(9600); // Initialize serial monitor
  servo.attach(13); // Attach servo to pin 13

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  stopMovement();
}

// Function for forward movement
void moveForward() {
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, HIGH);
}

// Function for backward movement
void moveBackward() {
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, LOW);
}

// Function for left turn
void turnLeft() {
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, LOW);
}

// Function for right turn
void turnRight() {
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, HIGH);
}

bool reverseAngle = false; // Boolean to control servo sweep direction
int angle = 90; // Current servo angle

int objectLeft = 0;
int objectLeftFront = 0;
int objectFront = 0;
int objectRightFront = 0;
int objectRight = 0;

// Function to perform servo sweep and measure distances
void radarSweep(int from, int to) {
  if (reverseAngle) {
    angle++; // Increase angle
  } else {
    angle--; // Decrease angle
  }
  
  // Reverse direction when reaching bounds
  if (angle >= to) {
    reverseAngle = false;
  }
  if (angle <= from) {
    reverseAngle = true;
  }

  // Move servo to the current angle
  servo.write(angle);

  // Perform distance reading at specific angles
  if (angle == 0 || angle == 45 || angle == 90 || angle == 135 || angle == 180) {
    int distance = ultrasonic.read(); // Read distance from ultrasonic sensor
    String position = "";

    if (angle == 0) {
      position = "Right";
      objectRight = distance;
    }
    if (angle == 45) {
      position = "Right Front";
      objectRightFront = distance;
    }
    if (angle == 90) {
      position = "Front";
      objectFront = distance;
    }
    if (angle == 135) {
      position = "Left Front";
      objectLeftFront = distance;
    }
    if (angle == 180) {
      position = "Left";
      objectLeft = distance;
    }

    Serial.println("Distance at " + position + ": " + String(distance));
  }
}

int radarInterval = 0; // Initial counter
int radarDelay = 10; // Delay in milliseconds for radar movement

// Function to control radar sweeping logic
void radarLoop() {
  if (radarInterval == radarDelay) {
    radarInterval = 0;
    radarSweep(0, 180); // Full sweep
    // radarSweep(45, 135); // Narrow front sweep
    // radarSweep(90, 90); // Single forward reading
  }
  radarInterval++;
}

unsigned long previousMillis = 0;
const long loopInterval = 1; // Main loop interval in milliseconds

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= loopInterval) {
    previousMillis = currentMillis;
    radarLoop();

    if (objectFront < 15 || objectLeftFront < 15 || objectRightFront < 15) {
      stopMovement();

      if (objectLeft < objectRight) {
        if (objectLeft < 15) {
          turnLeft();
        }
      } else {
        if (objectRight < 15) {
          turnRight();
        }
      }
    } else {
      if (objectFront > 15) {
        moveForward();
      }
    }
  }
}