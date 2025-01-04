#include <FastLED.h>

const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[kMatrixWidth * kMatrixHeight];
uint16_t noise[MAX_DIMENSION][MAX_DIMENSION];

void pixel(int x, int y, String color) {
  int x_ = 33 - x;
  int y_ = y;

  if (y_ > 24) {
    x_ = (96 - 3) + x_;
  } else if (y_ > 16) {
    x_ = (64 - 2) + x_;
  } else if (y_ > 8) {
    x_ = (32 - 1) + x_;
  }

  int i = (x_ * 8) - 7 + (y_ - 1);
  int i_ = i - 1;

  for (int k = 1; k < 32 * 4; k++) {
    if (i > 8 * ((k * 2) - 1) && i <= 8 * (((k * 2) - 1) + 1)) {
      i_ = (8 * (((k * 2) - 1) + 1)) - i + (8 * (((k * 2) - 1) + 1)) - 8;
      break;
    }
  }

  long number = strtol(&color[1], NULL, 16);

  leds[i_].red = (number >> 8) & 0xFF;
  leds[i_].green = number >> 16;
  leds[i_].blue = number & 0xFF;
}

void all(String color) {
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      pixel(x,y,color);
    }
  }
}

void randPixels() {
  for(int i=0;i<50;i++) {
    pixel(random(1,33),random(1,33),randColor());
  }
}

String randColor() {
    int numBytes = 0;
    int i = 0;
    int j = 0;
    String letters[16] = {"a", "b", "c", "d", "e", "f","1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    String randString = "";
    for(i = 1; i<=6; i++) {
      randString = randString + letters[random(0, 15)];
    }
    return "#"+randString;
}

int falled[32][32] = {0};

void resetFalled() {
    memset(falled, 0, sizeof(falled));
}

int fallint=0;
int fallx=0;
int points=0;
void fall() {

  if (fallx==0) {
    fallx=random(1,33);
    if (fallx==33) {
      fallx=32;
    }
    points++;
  }

  if (fallint==32) {
    falled[fallx-1][fallint-1]=1;
    fallx=random(1,33);
    if (fallx==33) {
      fallx=32;
    }
    fallint=0;
    points++;
  }

  if(falled[fallx-1][fallint-1+1]==0) {
    fallint++;
  } else if (falled[fallx-1][fallint-1+1]==1) {
    falled[fallx-1][fallint-1]=1;
    fallx=random(1,33);
    if (fallx==33) {
      fallx=32;
    }
    fallint=0;
    points++;
  }

  pixel(fallx,33-fallint,randColor());
  
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      if (falled[x-1][33-y-1]==1) {
        pixel(x,y,"#666666");
      }
    }
  }

  for(int y=1;y<=32;y++) {
    int countline=0;
    for(int x=1;x<=32;x++) {
      if (falled[x-1][33-y-1]==1) {
        countline++;
      }
    }
    if (countline>=32) {
      String c=randColor();
      for(int x=1;x<=32;x++) {
        falled[x-1][33-y-1]=0;
      }

      for(int x_=1;x_<=32;x_++) {
        for(int y_=1;y_<=32;y_++) {
          falled[x_-1][33-y_-1]=falled[x_-1][32-y_-1];
        }
        falled[x_-1][33-32-1]=0;
      }

    }
  } 

  if (points>512+256+128) {
    points=0;
    for(int x=1;x<=32;x++) {
      for(int y=1;y<=32;y++) {
        falled[x-1][y-1]=0;
      }
    } 
  }
  
}

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2811,2,RGB>(leds,NUM_LEDS);
    FastLED.setBrightness(10);
    resetFalled();
}

int count=0;
unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;  // interval at which to blink (milliseconds)

void loop() {

  unsigned long currentMillis = millis();
  if (previousMillis==0 || currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // colocar um evento aqui
    count++;
    Serial.println(count);
    if (count>120) {
        resetFalled();
    }
  }

  all("#000000");
  fall();
  FastLED.show();

}