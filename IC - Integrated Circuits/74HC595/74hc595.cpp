/*
    Demonstration of controlling a 74HC595 shift register using Arduino.

    The 74HC595 is an 8-bit shift register that outputs data to its pins in 
    parallel after receiving data serially. This example displays a counter 
    from 0 to 255 on LEDs connected to the shift register.

    Wiring:
    - DS (Data Pin)    -> Arduino Pin 11
    - SH_CP (Clock Pin) -> Arduino Pin 12
    - ST_CP (Latch Pin) -> Arduino Pin 8
    - Q0-Q7 -> LEDs or other output devices (with appropriate resistors)
*/

int latchPin = 8;  // Pin connected to ST_CP of 74HC595
int clockPin = 12; // Pin connected to SH_CP of 74HC595
int dataPin  = 11; // Pin connected to DS of 74HC595

void setup() {
    // Set control pins as outputs
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

void loop() {
    // Count from 0 to 255 and display the number on LEDs
    for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
        updateShiftRegister(numberToDisplay); // Update the register with the current number
        delay(500); // Pause for half a second
    }
}

/**
 * Update the 74HC595 with a new 8-bit value.
 * @param value The value to output (0-255).
 */
void updateShiftRegister(byte value) {
    digitalWrite(latchPin, LOW);                  // Disable output while updating
    shiftOut(dataPin, clockPin, MSBFIRST, value); // Shift out the bits to the shift register
    digitalWrite(latchPin, HIGH);                 // Latch the updated value to output
}

/**
 * Clear all outputs of the shift register (turn off all LEDs).
 */
void clearShiftRegister() {
    updateShiftRegister(0x00); // Send all zeros
}

/**
 * Set all outputs of the shift register to HIGH (turn on all LEDs).
 */
void setAllHigh() {
    updateShiftRegister(0xFF); // Send all ones
}

/**
 * Set a specific bit of the shift register to HIGH or LOW.
 * @param bit The bit to modify (0-7).
 * @param state The desired state (HIGH or LOW).
 */
void setBit(int bit, bool state) {
    static byte currentState = 0x00; // Store the current state of the shift register
    if (state) {
        currentState |= (1 << bit);  // Set the specified bit to 1
    } else {
        currentState &= ~(1 << bit); // Clear the specified bit to 0
    }
    updateShiftRegister(currentState); // Update the shift register
}

/**
 * Display a custom pattern on the shift register.
 * @param pattern A byte representing the custom pattern (e.g., 0b10101010).
 */
void displayPattern(byte pattern) {
    updateShiftRegister(pattern); // Update the shift register with the pattern
}

/**
 * Create a running light effect (one LED at a time).
 * @param delayTime Delay in milliseconds between shifts.
 */
void runningLight(int delayTime) {
    for (int i = 0; i < 8; i++) {
        displayPattern(1 << i); // Shift a single bit through the register
        delay(delayTime);
    }
}

/**
 * Create a ping-pong light effect (back and forth).
 * @param delayTime Delay in milliseconds between shifts.
 */
void pingPongLight(int delayTime) {
    for (int i = 0; i < 8; i++) {
        displayPattern(1 << i); // Shift a single bit forward
        delay(delayTime);
    }
    for (int i = 6; i > 0; i--) {
        displayPattern(1 << i); // Shift a single bit backward
        delay(delayTime);
    }
}
