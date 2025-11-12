//PWM ports for Arduino Pro Mini
int lr[] = {3, 9}; // Red LED PWM ports
int lg[] = {5, 10}; // Green LED PWM ports
int lb[] = {6, 11}; // Blue LED PWM ports

// Function to control RGB LED colors
// l: LED index, r: red intensity, g: green intensity, b: blue intensity
void rgb(int l, int r, int g, int b) {
    analogWrite(lr[l], r); // Set red intensity for LED l
    analogWrite(lg[l], g); // Set green intensity for LED l
    analogWrite(lb[l], b); // Set blue intensity for LED l
}

void setup() {
    Serial.begin(9600); // Initialize serial communication at 9600 baud
    for (int i = 0; i <= 1; i++) { // Loop through each LED
        pinMode(lr[i], OUTPUT); // Set red port as output
        pinMode(lg[i], OUTPUT); // Set green port as output
        pinMode(lb[i], OUTPUT); // Set blue port as output
    }
}

String value = ""; // String to store incoming serial data

void loop() {
    if (Serial.available() > 0) { // Check if data is available from serial input
        char c = Serial.read(); // Read a character from serial
        if (c) {
            if (c == ':') {
                value = ""; // Reset the value string when ':' is received
            } else if (c == '.') {
                // Expected format: r,g,b,led (e.g., 025,220,125,0 or 025,220,125,1)
                String r = value.substring(0, 3); // Extract red intensity
                String g = value.substring(4, 7); // Extract green intensity
                String b = value.substring(8, 11); // Extract blue intensity
                String l = value.substring(12, 13); // Extract LED index
                rgb(l.toInt(), r.toInt(), g.toInt(), b.toInt()); // Set LED color using parsed values
                Serial.print(r); // Print red intensity
                Serial.print(" - ");
                Serial.print(g); // Print green intensity
                Serial.print(" - ");
                Serial.print(b); // Print blue intensity
                Serial.print(" - ");
                Serial.print(l); // Print LED index
                Serial.println("");
                value = ""; // Reset value string after processing
            } else {
                value += c; // Append character to value string
            }
        }
    }
}
