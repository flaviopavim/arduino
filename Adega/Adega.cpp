void setup() {
    // Initial setup
    for (int i = 0; i < 6; i++) {
        pinMode(i, OUTPUT); // Set pins 0 to 5 as outputs to control valves
        digitalWrite(i, LOW); // Ensure all valves are initially closed
    }
    Serial.begin(9600); // Start serial communication for debugging
}

int drink = 0; // Indicates which drink is selected
bool start = false; // Indicates whether the process has started
int step = 0; // Current step in the process

// State of the valves (true = open, false = closed)
bool bottle[6] = {false, false, false, false, false, false};

// Function to reset all valves to the closed state
void resetBottles() {
    for (int i = 0; i < 6; i++) {
        bottle[i] = false; // Close all valves
        digitalWrite(i, LOW); // Set the valve pin to LOW (off)
    }
}

// Timing variables
unsigned long previousMillis = 0; // Stores the last recorded time
const long interval = 100; // Interval for actions in milliseconds (1/10 second)

void loop() {
    // Perform actions at fixed time intervals
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (start) {
            // Steps to prepare the selected drink
            if (drink == 1) { // Configuration for drink 1
                if (step == 0) {
                    resetBottles(); // Reset all valves
                    bottle[0] = true; // Open valve 1
                    bottle[1] = true; // Open valve 2
                } else if (step == 4) {
                    bottle[0] = false; // Close valve 1
                    bottle[1] = false; // Close valve 2
                    start = false; // End the drink process
                }
            } else if (drink == 2) { // Configuration for drink 2
                if (step == 0) {
                    resetBottles(); // Reset all valves
                    bottle[2] = true; // Open valve 3
                    bottle[3] = true; // Open valve 4
                } else if (step == 4) {
                    bottle[2] = false; // Close valve 3
                    bottle[3] = false; // Close valve 4
                    start = false; // End the drink process
                }
            }

            // Increment the step
            step++;

            // Activate or deactivate valves based on their current state
            for (int i = 0; i < 6; i++) {
                if (bottle[i]) {
                    digitalWrite(i, HIGH); // Turn the valve on
                } else {
                    digitalWrite(i, LOW); // Turn the valve off
                }
            }

        } else {
            step = 0; // Reset the step if the process is not active
        }
    }

    // Serial monitoring (optional)
    if (Serial.available() > 0) {
        char command = Serial.read(); // Read a command from the serial port
        if (command == '1') {
            drink = 1; // Select drink 1
            start = true; // Start the process
        } else if (command == '2') {
            drink = 2; // Select drink 2
            start = true; // Start the process
        } else if (command == 'r') {
            resetBottles(); // Manually reset all valves
            start = false; // End any ongoing process
        }
    }
}
