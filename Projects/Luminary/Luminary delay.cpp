// PWM pins for Arduino Pro Mini LEDs
int lr[] = {3, 9}; // Red LED pins
int lg[] = {5, 10}; // Green LED pins
int lb[] = {6, 11}; // Blue LED pins

// Function to set the RGB values for a specific LED
void rgb(int r_, int g_, int b_, int l) {
    analogWrite(lr[l], r_); // Set red channel for the selected LED
    analogWrite(lg[l], g_); // Set green channel for the selected LED
    analogWrite(lb[l], b_); // Set blue channel for the selected LED
}

// Function to set the same RGB values for both LEDs
void rgbs(int r_, int g_, int b_) {
    rgb(r_, g_, b_, 0); // Set RGB values for the first LED
    rgb(r_, g_, b_, 1); // Set RGB values for the second LED
}

void setup() {
    Serial.begin(9600); // Initialize serial communication
    // Configure all LED pins as OUTPUT
    for (int i = 0; i <= 1; i++) {
        pinMode(lr[i], OUTPUT); // Set red LED pin as output
        pinMode(lg[i], OUTPUT); // Set green LED pin as output
        pinMode(lb[i], OUTPUT); // Set blue LED pin as output
    }
}

// Function to make LEDs blink with specified RGB values
void blink_(int r_, int g_, int b_) {
    for (int i = 1; i <= 100; i++) { // Repeat blinking 100 times
        rgbs(r_, g_, b_); // Turn LEDs on with the specified RGB values
        delay(20); // Wait 20 milliseconds
        rgbs(0, 0, 0); // Turn LEDs off
        delay(20); // Wait 20 milliseconds
    }
}

// Function to display a sequence of colors
void colors() {
    rgbs(0, 0, 40); // Set LEDs to blue
    delay(1000); // Wait for 1 second
    rgbs(0, 40, 40); // Set LEDs to cyan
    delay(1000); // Wait for 1 second
    rgbs(0, 40, 0); // Set LEDs to green
    delay(1000); // Wait for 1 second
    rgbs(40, 40, 0); // Set LEDs to yellow
    delay(1000); // Wait for 1 second
    rgbs(40, 0, 0); // Set LEDs to red
    delay(1000); // Wait for 1 second
    rgbs(40, 0, 40); // Set LEDs to magenta
    delay(1000); // Wait for 1 second
}

int v = 90; // Brightness value for the LEDs

void loop() {
    blink_(v, 0, 0); // Blink LEDs in red
    blink_(v, 0, v); // Blink LEDs in magenta
    blink_(0, 0, v); // Blink LEDs in blue
    blink_(0, v, v); // Blink LEDs in cyan
    blink_(0, v, 0); // Blink LEDs in green
    blink_(v, v, 0); // Blink LEDs in yellow
    blink_(v, v, v); // Blink LEDs in white
}
