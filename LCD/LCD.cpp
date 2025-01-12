// Program: LCD 16x2 Display
// Author: MakerHero

// Load the LiquidCrystal library for controlling the LCD
#include <LiquidCrystal.h>

// Define the pins used to connect the LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
}

void loop() {
  // Clear the display
  lcd.clear();
  // Set the cursor to column 3, row 0 (top row)
  lcd.setCursor(3, 0);
  // Print the text "MakerHero" on the LCD
  lcd.print("MakerHero");
  // Set the cursor to column 3, row 1 (bottom row)
  lcd.setCursor(3, 1);
  // Print the text "LCD 16x2" on the LCD
  lcd.print(" LCD 16x2");
  // Wait for 5 seconds before performing the next action
  delay(5000);

  // Scroll text to the left 3 times
  for (int position = 0; position < 3; position++) {
    lcd.scrollDisplayLeft(); // Shift display content one position to the left
    delay(300); // Wait 300 milliseconds between shifts
  }

  // Scroll text to the right 6 times
  for (int position = 0; position < 6; position++) {
    lcd.scrollDisplayRight(); // Shift display content one position to the right
    delay(300); // Wait 300 milliseconds between shifts
  }
}
