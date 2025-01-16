#include "LEDMatrix.h"
#include "Effects.h"

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
