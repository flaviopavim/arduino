#include "LED.h"
#include "Effects.h"
#include "Draw.h"


// Matrix to track fixed pixels
int falled[32][32] = {0};

// Function to reset the matrix of fixed pixels
void resetFalled() {
    memset(falled, 0, sizeof(falled));
}

// Variables for the falling animation
int fallX = 0;
int fallY = 0;
int points = 0;

// Function to manage falling animation and updates
void fall() {
    // Generate a new falling position if needed
    if (fallX == 0) {
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure it's within bounds
        points++;
    }

    // Update position or finalize the fall
    if (fallY == 32 || falled[fallX - 1][fallY] == 1) {
        falled[fallX - 1][fallY - 1] = 1; // Mark the final position
        fallX = random(1, 33);
        if (fallX == 33) fallX = 32; // Ensure it's within bounds
        fallY = 0; // Reset falling position
        points++;
    } else {
        fallY++; // Move to the next position
    }

    // Draw the falling pixel
    pixel(fallX, 33 - fallY, randColor());

    // Draw the fixed pixels
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            if (falled[x][31 - y]) {
                pixel(x + 1, y + 1, "#666666");
            }
        }
    }

    // Check and remove complete rows
    for (int y = 0; y < 32; y++) {
        bool complete = true;
        for (int x = 0; x < 32; x++) {
            if (falled[x][y] == 0) {
                complete = false;
                break;
            }
        }

        if (complete) {
            // Clear the complete row
            for (int x = 0; x < 32; x++) {
                falled[x][y] = 0;
            }

            // Shift rows down
            for (int py = y; py > 0; py--) {
                for (int px = 0; px < 32; px++) {
                    falled[px][py] = falled[px][py - 1];
                }
            }

            // Clear the top row
            for (int x = 0; x < 32; x++) {
                falled[x][0] = 0;
            }
        }
    }

    // Reset the game if the point limit is exceeded
    if (points > 896) { // 512 + 256 + 128
        resetFalled();
        points = 0;
    }
}


// Variables for the ping-pong animation
int posX = random(1, 32);
int posY = 1;
bool directionX = true;
bool directionY = true;

// Function to draw a circle
void drawCircle(int centerX, int centerY, int radius) {
  all("#000000"); // Clear the screen

  // Draw the circle
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 32; x++) {
      // Calculate the distance from the pixel to the center
      int dx = x - centerX;
      int dy = y - centerY;
      int distanceSquared = dx * dx + dy * dy;

      // If within the radius, draw the pixel
      if (distanceSquared <= radius * radius) {
        pixel(x, y, "#ffffff");
      }
    }
  }
}

// Function to animate a ping-pong movement
void pingPong() {
  // Check boundaries and invert direction if needed
  if (posX <= 1 || posX >= 32) directionX = !directionX;
  if (posY <= 1 || posY >= 32) directionY = !directionY;

  // Update position based on direction
  posX += directionX ? 1 : -1;
  posY += directionY ? 2 : -1;

  // Constrain position to valid range
  posX = constrain(posX, 1, 32);
  posY = constrain(posY, 1, 32);

  // Draw the pixel at the new position with a random color
  pixel(posX, posY, randColor());
}

// Structure to hold information for each explosion
struct Explosion {
  int currentX;
  int currentY;
  int explosionStep;
  int offsetX1, offsetX2, offsetY1, offsetY2;
  int maxHeight;
  int maxExplosionRadius;
};

Explosion explosions[50]; // Array to store multiple explosions

// Initialize explosions with random values
void setupExplosions() {
  for (int i = 0; i < 50; i++) {
    explosions[i].currentX = random(1, 32);
    explosions[i].currentY = 1;
    explosions[i].explosionStep = 0;
    explosions[i].offsetX1 = 0;
    explosions[i].offsetX2 = 0;
    explosions[i].offsetY1 = 0;
    explosions[i].offsetY2 = 0;
    explosions[i].maxHeight = random(1, 32);
    explosions[i].maxExplosionRadius = 32;
  }
}

// Function to get the explosion color (fixed or random)
String getExplosionColor(int index) {
  if (index % 2 == 0) {
    if (index % 6 == 0) return "#ff0000"; // Red
    else if (index % 6 == 1) return "#00ff00"; // Green
    else if (index % 6 == 2) return "#0000ff"; // Blue
    else if (index % 6 == 3) return "#ffff00"; // Yellow
    else if (index % 6 == 4) return "#ff00ff"; // Magenta
    else return "#00ffff"; // Cyan
  }
  return randColor(); // Use a random color otherwise
}

// Function to animate an explosion
void explode(int index) {
  Explosion& explosion = explosions[index];

  // If not yet at the explosion point, animate the rising pixel
  if (explosion.currentY < explosion.maxHeight) {
    explosion.currentY++;
    pixel(explosion.currentX, explosion.currentY, getExplosionColor(index));
  } else {
    // Smaller scale for "X" expansion
    int scaledExplosion = explosion.explosionStep / 1.2;

    // Coordinates for the "X" expansion
    explosion.offsetX1 = int(floor(explosion.currentX + scaledExplosion));
    explosion.offsetX2 = int(floor(explosion.currentX - scaledExplosion));
    explosion.offsetY1 = int(floor(explosion.currentY + scaledExplosion));
    explosion.offsetY2 = int(floor(explosion.currentY - scaledExplosion));

    // Draw "X" expansion
    pixel(explosion.offsetX1, explosion.offsetY1, getExplosionColor(index));
    pixel(explosion.offsetX2, explosion.offsetY1, getExplosionColor(index));
    pixel(explosion.offsetX1, explosion.offsetY2, getExplosionColor(index));
    pixel(explosion.offsetX2, explosion.offsetY2, getExplosionColor(index));

    // Coordinates for cross expansion
    explosion.offsetX1 = explosion.currentX + explosion.explosionStep;
    explosion.offsetX2 = explosion.currentX - explosion.explosionStep;
    explosion.offsetY1 = explosion.currentY + explosion.explosionStep;
    explosion.offsetY2 = explosion.currentY - explosion.explosionStep;

    // Draw cross expansion
    pixel(explosion.offsetX1, explosion.currentY, getExplosionColor(index));
    pixel(explosion.offsetX2, explosion.currentY, getExplosionColor(index));
    pixel(explosion.currentX, explosion.offsetY1, getExplosionColor(index));
    pixel(explosion.currentX, explosion.offsetY2, getExplosionColor(index));

    // Increment explosion step
    explosion.explosionStep++;
    if (explosion.explosionStep > explosion.maxExplosionRadius) {
      // Reset after explosion
      explosion.explosionStep = 0;
      explosion.offsetX1 = 0;
      explosion.offsetX2 = 0;
      explosion.offsetY1 = 0;
      explosion.offsetY2 = 0;
      explosion.currentX = random(1, 32);
      explosion.currentY = 1;
      explosion.maxHeight = random(1, 32);
      explosion.maxExplosionRadius = 32;
    }
  }
}

void fireworks() {
  for (int i = 0; i < 8; i++) {
    explode(i);
  }
}


void randPixels() {
  for (int i = 0; i < 50; i++) {
    pixel(random(1, 32), random(1, 32), randColor());
  }
}

unsigned long previousMillis = 0;
const long interval = 1000*10; // 10 seconds
int effect_number=0;

int pickEffect() {
  unsigned long currentMillis = millis();
  if (previousMillis == 0 || currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      effect_number++;
      if (effect_number>4) {
        effect_number=1;
      }
  }
  return effect_number;
}













// Matrix to track falling raindrops
int raindrops[32][32] = {0};

// Function to reset the matrix of raindrops
void resetRaindrops() {
    memset(raindrops, 0, sizeof(raindrops));
}

// Variables for the rain animation
int rainX[5] = {0}; // Array to hold X positions for raindrops
int rainY[5] = {0}; // Array to hold Y positions for raindrops

// Function to simulate the rain
void rain() {
    // Loop through all raindrops
    for (int i = 0; i < 5; i++) {
        // If a raindrop reaches the bottom or is blocked, reset it to the top
        if (rainY[i] >= 32 || raindrops[rainX[i] - 1][rainY[i]] == 1) {
            rainX[i] = random(1, 33);
            if (rainX[i] == 33) rainX[i] = 32; // Ensure it's within bounds
            rainY[i] = 0; // Reset Y position to top
        } else {
            rainY[i]++; // Move the raindrop down
        }

        // Draw the raindrop as a blue pixel
        pixel(rainX[i], 33 - rainY[i], "#0000ff");

        // Mark the raindrop as fixed in the matrix
        raindrops[rainX[i] - 1][rainY[i] - 1] = 1;
    }

    // Draw the fixed raindrops
    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            if (raindrops[x][31 - y]) {
                pixel(x + 1, y + 1, "#0000ff");
            }
        }
    }

    // Reset the raindrop matrix if points exceed a certain threshold
    // This ensures that the rain keeps falling without being blocked by the fixed pixels
    if (points > 896) { // 512 + 256 + 128
        resetRaindrops();
        points = 0;
    }
}



void effects() {
  int effect_number=pickEffect();
  if (effect_number==1) {
    fireworks();
    //fall();
  } else if (effect_number==2) {
    pingPong();
  } else if (effect_number==3) {
    //rain();
  } else if (effect_number==4) {
    randPixels();
  }
}