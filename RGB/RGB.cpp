// LED 1 pins
int R1 = 3; // Red channel for LED 1
int G1 = 5; // Green channel for LED 1
int B1 = 6; // Blue channel for LED 1

// LED 2 pins
int R2 = 9;  // Red channel for LED 2
int G2 = 10; // Green channel for LED 2
int B2 = 11; // Blue channel for LED 2

// Brightness values for LED 1 (initially set to 0)
int brightness_r1 = 0; // Red brightness for LED 1
int brightness_g1 = 0; // Green brightness for LED 1
int brightness_b1 = 0; // Blue brightness for LED 1

// Brightness values for LED 2 (initially set to 0)
int brightness_r2 = 0; // Red brightness for LED 2
int brightness_g2 = 0; // Green brightness for LED 2
int brightness_b2 = 0; // Blue brightness for LED 2

// Function to apply the brightness values to the LEDs
void apply() {
  // Set the brightness for LED 1
  analogWrite(R1, brightness_r1);
  analogWrite(G1, brightness_g1);
  analogWrite(B1, brightness_b1);

  // Set the brightness for LED 2
  analogWrite(R2, brightness_r2);
  analogWrite(G2, brightness_g2);
  analogWrite(B2, brightness_b2);
}

// Amount to increase or decrease the brightness in each loop
int fadeAmount = 5;

void setup() {
  // Set all LED pins as OUTPUT
  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(B1, OUTPUT);

  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(B2, OUTPUT);
}

void loop() {
  // Apply the current brightness values to the LEDs
  apply();

  // Adjust brightness for LED 1 red channel
  brightness_r1 = brightness_r1 + fadeAmount;
  if (brightness_r1 <= 0 || brightness_r1 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Adjust brightness for LED 2 green channel
  brightness_g2 = brightness_g2 + fadeAmount;
  if (brightness_g2 <= 0 || brightness_g2 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Adjust brightness for LED 2 blue channel
  brightness_b2 = brightness_b2 + fadeAmount;
  if (brightness_b2 <= 0 || brightness_b2 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Adjust brightness for LED 2 red channel
  brightness_r2 = brightness_r2 + fadeAmount;
  if (brightness_r2 <= 0 || brightness_r2 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Adjust brightness for LED 2 green channel (repeated adjustment)
  brightness_g2 = brightness_g2 + fadeAmount;
  if (brightness_g2 <= 0 || brightness_g2 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Adjust brightness for LED 2 blue channel (repeated adjustment)
  brightness_b2 = brightness_b2 + fadeAmount;
  if (brightness_b2 <= 0 || brightness_b2 >= 255) {
    fadeAmount = -fadeAmount; // Reverse direction if limit is reached
  }

  // Delay to slow down the fading effect
  delay(30);
}
