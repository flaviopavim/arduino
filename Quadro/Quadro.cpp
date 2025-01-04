#include <FastLED.h>

const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 32;

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[kMatrixWidth * kMatrixHeight];
uint16_t noise[MAX_DIMENSION][MAX_DIMENSION];

void point(int i, String color) {
  int i_=i-1;
  for (int k=1;k<32*4;k++) {
    if (i>8*((k*2)-1) && i<=8*(((k*2)-1)+1)) {
      i_=(8*(((k*2)-1)+1))-i+(8*(((k*2)-1)+1))-8;
    }
  }

  String hexstring = color;
  long number = (long) strtol( &hexstring[1], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  //ta invertido g com r
  //fiz essa gambiarra pra deixar rgb senão jajá to doido kkkk
  leds[i_].red   = g;
  leds[i_].green = r;
  leds[i_].blue  = b;
}

void led(int x, int y, String color) {
  point((x*8)-7+(y-1),color);
}



void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811,2,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(10);
}

void pixel(int x,int y, String color) {
  int x_=x;
  int y_=33-y;
  if (y_>24) {
    x_=(96-3)+x;
  } else if (y_>16) {
    x_=(64-2)+x;
  } else if (y_>8) {
    x_=(32-1)+x;
  }
  led(x_,y_,color);
}

void p(int x,int y, String color) {
  pixel(33-x,33-y,color);
}

int Matrix[16][12]={
  {0,0,0,4,4,4,4,4,0,0,0,0},
  {0,0,4,4,4,4,4,4,4,4,4,0},
  {0,0,1,1,1,3,3,5,3,0,0,0},
  {0,1,3,1,3,3,3,5,3,3,3,0},
  {0,1,3,1,1,3,3,3,5,3,3,3},
  {0,0,1,3,3,3,3,5,5,5,5,0},
  {0,0,0,3,3,3,3,3,3,0,0,0},
  {0,0,4,4,2,4,4,2,4,4,0,0},
  {0,4,4,4,2,4,4,2,4,4,4,0},
  {4,4,4,4,2,2,2,2,4,4,4,4},
  {3,3,4,2,3,2,2,3,2,4,3,3},
  {3,3,3,2,2,2,2,2,2,3,3,3},
  {3,3,2,2,2,2,2,2,2,2,3,3},
  {0,0,2,2,2,0,0,2,2,2,0,0},
  {0,1,1,1,0,0,0,0,1,1,1,0},
  {1,1,1,1,0,0,0,0,1,1,1,1}
};

void black() {
  for(int i=0;i<32*4*8;i++) {
    leds[i].red   = 0;
    leds[i].green = 0;
    leds[i].blue  = 0;
  }
}

void white() {
  for(int i=0;i<32*4*8;i++) {
    leds[i].red   = 30;
    leds[i].green = 30;
    leds[i].blue  = 30;
  }
}

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 1000;  // interval at which to blink (milliseconds)

int x=16;
int y=1;
int c=1;
int r=1;

void randPixels() {
  for(int i=0;i<50;i++) {
    p(random(1,33),random(1,33),randColor());
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

void border() {
  for(int x=1;x<=32;x++) {
    p(x,1,"#FFFFFF");  
    p(x,32,"#FFFFFF");  
  }
  for(int y=1;y<=32;y++) {
    p(1,y,"#FFFFFF");  
    p(32,y,"#FFFFFF");  
  }
}

String colors[7]={
  "#DA0000","#DA6600","#DADA00","#00DA00",
  "#0000FF","#000099","#712580"
};

int falled[32][32]={
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


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

  p(fallx,33-fallint,randColor());
  
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      if (falled[x-1][33-y-1]==1) {
        p(x,y,"#666666");
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

void all(String color) {
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,color);
    }
  }
}

int count=0;

void loop() {

  unsigned long currentMillis = millis();
  if (previousMillis==0 || currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // colocar um evento aqui
    count++;
    Serial.println(count);
    if (count>120) {
        ESP.restart();
    }
  }

  black();

  fall();

  FastLED.show();

}
