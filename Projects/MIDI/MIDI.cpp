// MIDI note velocity (must be between 0 and 127). 
// Higher values usually result in louder sounds.
int velocity = 100; 

// MIDI command for "Note ON" (144 = 10010000 in binary).
int noteON = 144;

// MIDI command for "Note OFF" (128 = 10000000 in binary).
int noteOFF = 128;

/**
 * Sends a MIDI message via the serial connection.
 * 
 * @param command The MIDI command (e.g., noteON or noteOFF).
 * @param MIDInote The MIDI note number (pitch) to send (e.g., 60 for Middle C).
 * @param MIDIvelocity The velocity of the note (loudness).
 */
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
    Serial.write(command);     // Send the MIDI command (Note ON or Note OFF).
    Serial.write(MIDInote);    // Send the MIDI note (pitch) data.
    Serial.write(MIDIvelocity);// Send the MIDI velocity data.
}

void setup() {
    // Initialize the serial connection at 115,200 baud rate for MIDI communication.
    Serial.begin(115200);
}

void loop() {
    // Iterate through a range of notes, from note 50 (D3) to note 69 (A4).
    for (int note = 50; note < 70; note++) {
        MIDImessage(noteON, note, velocity); // Turn the current note ON.
        delay(300);                          // Hold the note for 300 milliseconds.
        MIDImessage(noteOFF, note, velocity);// Turn the current note OFF.
        delay(200);                          // Wait 200 milliseconds before the next note.
    }
}
