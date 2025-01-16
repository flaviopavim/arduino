#include <Arduino.h>

#include "WiFiConnection.h"  // Includes the header file for WiFi connection handling
#include "API.h"             // Includes the header file for API interactions
#include "LEDMatrix.h"       // Includes the header file for controlling the LED matrix
#include "Draw.h"            // Includes the header file for drawing functions
#include "MyClock.h"         // Includes the header file for clock-related functionalities

// Uncommented declaration for `actualColor` could define a default color for operations
// String actualColor = "#ffffff";

int count = 0;                   // Counter for tracking time or operations
unsigned long previousMillis_ = 0; // Tracks the last time `loopClock` was executed
const long interval_ = 1000;     // Time interval in milliseconds (1 second)

// Function to manage clock logic and periodic updates
void loopClock() {
    unsigned long currentMillis_ = millis(); // Gets the current time in milliseconds

    // Executes only if the interval between updates has passed
    if (previousMillis_ == 0 || currentMillis_ - previousMillis_ >= interval_) {
        previousMillis_ = currentMillis_; // Updates the last execution time

        // Increments the seconds counter
        seconds++;
        if (seconds == 60) {      // Handles minute rollover
            seconds = 0;
            minutes++;
            if (minutes == 60) {  // Handles hour rollover
                minutes = 0;
                hours++;
                if (hours == 24) { // Handles day rollover
                    hours = 0;
                }
            }
        }

        // Uncommented `count` logic could serve as a periodic task tracker
        // count++;
        // Serial.println(count);

        // Every 15 minutes, fetches the time again
        if (count > 60 * 15) { 
            count = 0;       // Resets the counter
            // resetFalled();  // Resets additional states (uncomment if needed)

            // Calls `setTime` to synchronize time
            setTime();
        }
    }
}