#include <FastLED.h>  // Include FastLED library for controlling LEDs.

// --- Configuration Constants ---
#define DATA_PIN 4           // Pin connected to the LED strip (WS2812B type).
#define LED_TYPE WS2812B     // Type of LED strip.
#define COLOR_ORDER GRB      // Color order for the LED strip.
#define NUM_LEDS 256         // Number of LEDs in the strip.
#define BRIGHTNESS 96        // Brightness level (0-255).
#define FRAMES_PER_SECOND 120 // Target frame rate for animations.

// Array to store the current state of the LEDs.
CRGB leds[NUM_LEDS];

// --- Function Prototypes ---
void nextPattern();
void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void bpm();
void juggle();
void addGlitter(fract8 chanceOfGlitter);

// --- Animation Patterns ---
typedef void (*SimplePatternList[])(); // Array of pattern functions.
SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};

uint8_t gCurrentPatternNumber = 0; // Index of the current pattern.
uint8_t gHue = 0;                  // Base color for animations.

// --- Setup Function ---
void setup() {
  delay(3000); // Delay for 3 seconds (useful for recovering after a crash).

  // Configure the LED strip with FastLED.
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);

  // Set the global brightness level.
  FastLED.setBrightness(BRIGHTNESS);
}

// --- Main Loop ---
void loop() {
  // Call the current pattern function to update the LED array.
  gPatterns[gCurrentPatternNumber]();

  // Show the updated LED states on the strip.
  FastLED.show();

  // Maintain the target frame rate.
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // Gradually rotate the base color.
  EVERY_N_MILLISECONDS(20) { gHue++; }

  // Automatically switch to the next pattern every 10 seconds.
  EVERY_N_SECONDS(10) { nextPattern(); }
}

// --- Helper Functions ---
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Switch to the next pattern in the list.
void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

// --- Animation Patterns ---
void rainbow() {
  // Displays a rainbow effect using FastLED's built-in function.
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // Rainbow pattern with random glitter added.
  rainbow();
  addGlitter(80); // 80% chance of adding glitter.
}

void addGlitter(fract8 chanceOfGlitter) {
  // Adds a random white sparkle to the LEDs.
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti() {
  // Displays a confetti effect with random colored speckles.
  fadeToBlackBy(leds, NUM_LEDS, 10); // Gradually fades LEDs to black.
  int pos = random16(NUM_LEDS);     // Choose a random LED position.
  leds[pos] += CHSV(gHue + random8(64), 200, 255); // Add a colored speckle.
}

void sinelon() {
  // Creates a colored dot sweeping back and forth with fading trails.
  fadeToBlackBy(leds, NUM_LEDS, 20); // Fade existing colors.
  int pos = beatsin16(13, 0, NUM_LEDS - 1); // Calculate dot position.
  leds[pos] += CHSV(gHue, 255, 192);       // Add a moving colored dot.
}

void bpm() {
  // Displays color patterns pulsing at a defined Beats-Per-Minute (BPM).
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p; // Use a predefined color palette.
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255); // Calculate beat intensity.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // Displays 8 colored dots weaving in and out of sync.
  fadeToBlackBy(leds, NUM_LEDS, 20); // Fade existing colors.
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255); // Add dots.
    dothue += 32; // Shift hue for each dot.
  }
}
