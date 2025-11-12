/*
    Hello World for IC TLC5940
    This code demonstrates basic usage of the TLC5940 library to control 
    the brightness of LEDs connected to the IC's 16 channels.
*/

#include "Tlc5940.h" 

void setup() {
    // Initialize the TLC5940 with a default grayscale value of 0 (off).
    Tlc.init(0);
}

void loop() {
    // Loop through all 16 channels of the TLC5940.
    for (int i = 0; i < 16; i++) {
        // Gradually increase the brightness of the current channel.
        for (int a = 0; a <= 4095; a++) {
            Tlc.set(i, a);     // Set the brightness of channel 'i' to 'a'.
            Tlc.update();      // Update the output to reflect the changes.
            delay(10);         // Small delay for a smooth fade-in effect.
        }
        delay(50);             // Hold the maximum brightness momentarily.

        Tlc.clear();           // Clear all channels (turn LEDs off).
        Tlc.update();          // Apply the changes.
        delay(50);             // Short pause before moving to the next channel.
    }
}

// Function to set a specific channel to a given brightness value (0-4095).
void setChannel(int channel, int value) {
    Tlc.set(channel, value);  // Set the brightness of the specified channel.
    Tlc.update();             // Update the output to reflect the change.
}

// Function to set all channels to the same brightness value (0-4095).
void setAllChannels(int value) {
    for (int i = 0; i < 16; i++) {
        Tlc.set(i, value);    // Set each channel to the specified value.
    }
    Tlc.update();             // Apply the changes to all channels.
}

// Function to fade a specific channel from one brightness level to another.
void fadeChannel(int channel, int startValue, int endValue, int stepDelay) {
    int step = (startValue < endValue) ? 1 : -1;  // Determine fade direction.
    for (int value = startValue; value != endValue; value += step) {
        Tlc.set(channel, value);  // Update the brightness for the specified channel.
        Tlc.update();
        delay(stepDelay);         // Pause between steps for a smooth transition.
    }
}

// Function to fade all channels simultaneously to a specified brightness.
void fadeAllChannels(int endValue, int stepDelay) {
    int step = (Tlc.get(0) < endValue) ? 1 : -1;  // Determine fade direction.
    for (int value = Tlc.get(0); value != endValue; value += step) {
        for (int i = 0; i < 16; i++) {
            Tlc.set(i, value);    // Update all channels to the same value.
        }
        Tlc.update();
        delay(stepDelay);         // Pause between steps for a smooth fade.
    }
}