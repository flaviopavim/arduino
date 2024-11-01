#include <FastLED.h>

CRGB leds[256];
void setup() {
  FastLED.addLeds<NEOPIXEL, 2>(leds, 256);
}

void a(int n) {
  //A
  n=n*40;
  leds[n+0] = CRGB::Blue;
  leds[n+1] = CRGB::Blue;
  leds[n+2] = CRGB::Blue;
  leds[n+3] = CRGB::Blue;
  leds[n+4] = CRGB::Blue;
  leds[n+5] = CRGB::Blue;
  leds[n+6] = CRGB::Blue;

  leds[n+8] = CRGB::Blue;
  leds[n+11] = CRGB::Blue;
  leds[n+20] = CRGB::Blue;
  leds[n+23] = CRGB::Blue;

  leds[n+25] = CRGB::Blue;
  leds[n+26] = CRGB::Blue;
  leds[n+27] = CRGB::Blue;
  leds[n+28] = CRGB::Blue;
  leds[n+29] = CRGB::Blue;
  leds[n+30] = CRGB::Blue;
  leds[n+31] = CRGB::Blue;
}

void b(int n) {
  //B
  n=n*40;
  leds[n+0] = CRGB::Red;
  leds[n+1] = CRGB::Red;
  leds[n+2] = CRGB::Red;
  leds[n+3] = CRGB::Red;
  leds[n+4] = CRGB::Red;
  leds[n+5] = CRGB::Red;
  leds[n+6] = CRGB::Red;
  leds[n+7] = CRGB::Red;

  leds[n+8] = CRGB::Red;
  leds[n+15] = CRGB::Red;
  leds[n+16] = CRGB::Red;
  leds[n+19] = CRGB::Red;
  leds[n+20] = CRGB::Red;
  leds[n+23] = CRGB::Red;

  leds[n+25] = CRGB::Red;
  leds[n+26] = CRGB::Red;
  leds[n+29] = CRGB::Red;
  leds[n+30] = CRGB::Red;
}

void d(int n) {
  //D
  n=n*40;
  leds[n+0] = CRGB::Green;
  leds[n+1] = CRGB::Green;
  leds[n+2] = CRGB::Green;
  leds[n+3] = CRGB::Green;
  leds[n+4] = CRGB::Green;
  leds[n+5] = CRGB::Green;
  leds[n+6] = CRGB::Green;
  leds[n+7] = CRGB::Green;

  leds[n+8] = CRGB::Green;
  leds[n+15] = CRGB::Green;
  leds[n+16] = CRGB::Green;
  leds[n+23] = CRGB::Green;

  leds[n+25] = CRGB::Green;
  leds[n+26] = CRGB::Green;
  leds[n+27] = CRGB::Green;
  leds[n+28] = CRGB::Green;
  leds[n+29] = CRGB::Green;
  leds[n+30] = CRGB::Green;
}

void e(int n) {
  //E
  n=n*40;
  leds[n+0] = CRGB::Blue;
  leds[n+1] = CRGB::Blue;
  leds[n+2] = CRGB::Blue;
  leds[n+3] = CRGB::Blue;
  leds[n+4] = CRGB::Blue;
  leds[n+5] = CRGB::Blue;
  leds[n+6] = CRGB::Blue;
  leds[n+7] = CRGB::Blue;
  leds[n+8] = CRGB::Blue;

  leds[n+12] = CRGB::Blue;
  leds[n+15] = CRGB::Blue;
  leds[n+16] = CRGB::Blue;
  leds[n+19] = CRGB::Blue;

  leds[n+23] = CRGB::Blue;
  leds[n+24] = CRGB::Blue;
  leds[n+31] = CRGB::Blue;
}

void n(int n_) {
  //N
  n_=n_*40;
  leds[n_+0] = CRGB::Red;
  leds[n_+1] = CRGB::Red;
  leds[n_+2] = CRGB::Red;
  leds[n_+3] = CRGB::Red;
  leds[n_+4] = CRGB::Red;
  leds[n_+5] = CRGB::Red;
  leds[n_+6] = CRGB::Red;
  leds[n_+7] = CRGB::Red;

  leds[n_+10] = CRGB::Red;
  leds[n_+20] = CRGB::Red;

  leds[n_+24] = CRGB::Red;
  leds[n_+25] = CRGB::Red;
  leds[n_+26] = CRGB::Red;
  leds[n_+27] = CRGB::Red;
  leds[n_+28] = CRGB::Red;
  leds[n_+29] = CRGB::Red;
  leds[n_+30] = CRGB::Red;
  leds[n_+31] = CRGB::Red;
}

void r(int n) {
  //R
  n=n*40;

  leds[n+0] = CRGB::Green;
  leds[n+1] = CRGB::Green;
  leds[n+2] = CRGB::Green;
  leds[n+3] = CRGB::Green;
  leds[n+4] = CRGB::Green;
  leds[n+5] = CRGB::Green;
  leds[n+6] = CRGB::Green;
  leds[n+7] = CRGB::Green;

  leds[n+8] = CRGB::Green;
  leds[n+11] = CRGB::Green;
  leds[n+19] = CRGB::Green;
  leds[n+20] = CRGB::Green;

  leds[n+23] = CRGB::Green;
  leds[n+25] = CRGB::Green;
  leds[n+26] = CRGB::Green;
  leds[n+29] = CRGB::Green;
  leds[n+30] = CRGB::Green;
  leds[n+31] = CRGB::Green;
}

void black() {
  for(int i = 0; i < 256; i++) {
      leds[i] = CRGB::Black;
    }
}


void randLED() {
  int n=random(0, 255);
  int c=random(0,6);
  if (c==0) {
    leds[n] = CRGB::Red;
  } else if (c==1) {
    leds[n] = CRGB::Green;
  } else if (c==2) {
    leds[n] = CRGB::Blue;
  } else if (c==3) {
    leds[n] = CRGB::Yellow;
  } else if (c==4) {
    leds[n] = CRGB::Purple;
  } else if (c==5) {
    leds[n] = CRGB::Orange;
  } else if (c==6) {
    leds[n] = CRGB::White;
  }
}


void brenda() {
  for(int i = 0; i <= 5; i++) {
    black();
    if (i==0) {
      b(0);
    } else if (i==1) {
      r(1);
    } else if (i==2) {
      e(2);
    } else if (i==3) {
      n(3);
    } else if (i==4) {
      d(4);
    } else if (i==5) {
      a(5);
    }
    FastLED.show();
    delay(150);
  }
}


void loop() {
  //brenda();
  black();
  for(int i = 0; i <= 100; i++) {
    randLED();
  }
  FastLED.show();
  delay(10);
}
