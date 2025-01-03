/**********************************************************************************************/

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

//usado no efeito neve e chuva
int counts[32]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void randomize() {
  for(int x=1;x<=32;x++) {
    counts[x]=random(1,32);
  }
}

void setup() {
  Serial.begin(9600);
  setupWifi();
  setupGyro();
  FastLED.addLeds<WS2811,2,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(10);
  randomize();
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

  if (position==-90) {
    int x_=33-y;
    int y_=33-x;
    x=33-x_;
    y=y_;
  } else if (position==90) {
    int x_=y;
    int y_=x;
    x=33-x_;
    y=y_;
  } else if (position==180) {
    x=33-x;
    y=33-y;
  }

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
    //leds[i] = CHSV(0,0,0);
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

    //leds[i] = CHSV(120,120,120);
  }
}

void draw() {
  //white();
  for(int x=1;x<=12;x++) {
    for(int y=1;y<=16;y++) {
      if (Matrix[y-1][x-1]>0) {
        String c="#000000";

        if (Matrix[y-1][x-1]==1) {
          c="#6A2400"; //FFD910 sapatos e cabelo
        } else if (Matrix[y-1][x-1]==2) {
          c="#0001FC"; //macacão
        } else if (Matrix[y-1][x-1]==3) {
          c="#FFD987"; //corpo e rosto
        } else if (Matrix[y-1][x-1]==4) {
          c="#FF0000"; //camiseta e chapéu
        } else if (Matrix[y-1][x-1]==5) {
          c="#222222"; //barba
        }

        //o 10 é deslocamento
        p(x+10,17-y,c);
      }
    }
  }
}

void bars() {
  for(int x=1;x<=32;x++) {
    int count=random(14,22);
    for(int y=1;y<count-4;y++) {
      p(x,y,"#AAAAAA");
    }
    if (count>4) {
      p(x,count-4,"#999900");
    }
    if (count>3) {
      p(x,count-3,"#999900");
    }
    if (count>2) {
      p(x,count-2,"#999900");
    }
    if (count>1) {
      p(x,count-1,"#990000");
    }
    p(x,count,"#990000");
  }
}

void snow() {
  for(int x=1;x<=32;x++) {
    p(x,33-counts[x-1],"#999999");
    if (floor(random(3,5))==3) {
      if (counts[x-1]>=32) {
        counts[x-1]=0;
      }
      counts[x-1]++;
    }
  }
}

void rain() {
  for(int x=1;x<=32;x++) {
    for(int y=counts[x-1]-1;y<=counts[x-1];y++) {
      p(x,33-y,"#999999");
      if (floor(random(3,5))==3) {
        p(x,33-y,"#cccccc");
        if (counts[x-1]>=32) {
          counts[x-1]=0;
        }
        counts[x-1]++;
      }
    }
  }
}


unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 30000;  // interval at which to blink (milliseconds)


unsigned long previousMillisClock = 0;  // will store last time LED was updated
const long intervalClock = 1000;  // interval at which to blink (milliseconds)

int effect=0;

int x=16;
int y=1;
int c=1;
int r=1;
bool right=false;
bool jumping=false;

void ball() {
  p(x,y,"#999999");

  c++;
  if (c>4) {
    c=1;
  }

  if (right) {
    x+=r;
  } else {
    x-=r;
  }
  if (x<1) {
    x=1;
    right=true;
    r=random(1,3);
  } else if (x>32) {
    x=32;
    right=false;
    r=random(1,3);
  }

  if (jumping) {
    y++;
  } else {
    y--;
  }
  if (y<1) {
    y=1;
    jumping=true;
    r=random(1,2);
  } else if (y>32) {
    y=32;
    jumping=false;
    r=random(1,2);
  }
}

void randPixels() {
  for(int i=0;i<50;i++) {
    p(random(1,33),random(1,33),randColor());
  }
}

void toggle() {
  if (design=="christmas") {
    design="mario";
  } else if (design=="mario") {
    design="random";
  } else if (design=="random") {
    design="rainbow";
  } else if (design=="rainbow") {
    design="ball";
  } else if (design=="ball") {
    design="fall";
  } else if (design=="fall") {
    design="bars";
  } else if (design=="bars") {
    design="lunch";
  } else if (design=="lunch") {
    design="square";
  } else if (design=="square") {
    design="clock";
  } else if (design=="clock") {
    design="christmas";
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

String randTreeColor() {
  //efeito (hack) do pisca-pisca na árvore
  int r=random(1,5);
  if (r==1) {
    return randColor();
  } else {
    return "#00FF00";
  }
}

void tree() {

  //tronco
  p(16,2,"#DDDD00");
  p(17,2,"#DDDD00");
  p(16,3,"#DDDD00");
  p(17,3,"#DDDD00");
  p(16,4,"#DDDD00");
  p(17,4,"#DDDD00");
  p(16,5,"#DDDD00");
  p(17,5,"#DDDD00");

  for(int i=6;i<=27;i++) {
    if (i<=7) {
      p(6,i,randTreeColor());
      p(27,i,randTreeColor());
    }
    if (i<=9) {
      p(7,i,randTreeColor());
      p(26,i,randTreeColor());
    }
    if (i<=11) {
      p(8,i,randTreeColor());
      p(25,i,randTreeColor());
    }
    if (i<=13) {
      p(9,i,randTreeColor());
      p(24,i,randTreeColor());
    }
    if (i<=15) {
      p(10,i,randTreeColor());
      p(23,i,randTreeColor());
    }
    if (i<=17) {
      p(11,i,randTreeColor());
      p(22,i,randTreeColor());
    }
    if (i<=19) {
      p(12,i,randTreeColor());
      p(21,i,randTreeColor());
    }
    if (i<=21) {
      p(13,i,randTreeColor());
      p(20,i,randTreeColor());
    }
    if (i<=23) {
      p(14,i,randTreeColor());
      p(19,i,randTreeColor());
    }
    if (i<=25) {
      p(15,i,randTreeColor());
      p(18,i,randTreeColor());
    }
    if (i<=27) {
      p(16,i,randTreeColor());
      p(17,i,randTreeColor());
    }
  }

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
int n_rainbow=0;
void rainbow() {

  if (position==0) {
     position=90;
  } else if (position==90) {
    position=180;
  } else if (position==180) {
    position=-90;
  } else if (position==-90) {
    position=0;
  }

  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }
  //vermelho
  for(int x=1;x<=5;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }
  //laranja
  for(int x=6;x<=10;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

  //amarelo
  for(int x=11;x<=14;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

  //verde
  for(int x=15;x<=18;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

  //azul
  for(int x=19;x<=22;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

  //anil
  for(int x=23;x<=27;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

  //violeta
  for(int x=28;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,colors[n_rainbow]);
    }
  }
  n_rainbow++;
  if (n_rainbow>6) {
    n_rainbow=0;
  }

}

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
        //p(x,y,c);
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

void lineX(int y, int from, int to, String color) {
  for(int i=from; i<=to;i++) {
    p(i,y,color);
  }
}
void lineY(int x, int from, int to, String color) {
  for(int i=from; i<=to;i++) {
    p(x,i,color);
  }
}

void square(int from_x, int to_x, int from_y, int to_y, String color) {
  for(int i=from_x; i<=to_x;i++) {
    p(i,from_y,color);
  }
  for(int i=from_x; i<=to_x;i++) {
    p(i,to_y,color);
  }


  for(int i=from_y; i<=to_y;i++) {
    p(from_x,i,color);
  }
  for(int i=from_y; i<=to_y;i++) {
    p(to_x,i,color);
  }
}

int count_square=0;
int count_square_color=0;
bool bool_square=false;
void squareEffect() {
  count_square--;
  if (count_square<0) {
    count_square=15;
    count_square_color++;
  }
  if (count_square>15) {
    count_square=0;
    count_square_color++;
  }
  
  if (count_square_color>5) {
    count_square_color=0;
    bool_square=!bool_square;
  }
  String c="";
  if (count_square_color==0) {
    c="#DDDDDD";
  } else if (count_square_color==1) {
    c="#0000DD";
  } else if (count_square_color==2) {
    c="#00DDDD";
  } else if (count_square_color==3) {
    c="#DDDD00";
  } else if (count_square_color==4) {
    c="#DD0000";
  } else if (count_square_color==5) {
    c="#DD00DD";
  }
  square(1+count_square,32-count_square,1+count_square,32-count_square,c);
}


void all(String color) {
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      p(x,y,color);
    }
  }
}

void loop() {
  loopGyro();

  unsigned long currentMillis = millis();
  if (previousMillis==0 || currentMillis - previousMillis >= interval) {
    colorRand=randColor();
    loopWifi();
    previousMillis = currentMillis;
    effect++;
    if (effect>6) {
      effect=0;
    }
  }
  black();

  fall();

  FastLED.show();

}
