#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//String api = "http://192.168.69.102/";
String api = "http://whitehats.com.br/esp/";
const char *wifi="AMNET85_0969";
const char *pass="qweasd01";

ESP8266WiFiMulti WiFiMulti;

String datetime="";

void setupWifi() {
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(wifi, pass);
    /*
    pinMode(A0, OUTPUT);
    pinMode(D0, OUTPUT);
    digitalWrite(A0, HIGH);
    digitalWrite(D0, HIGH);

    */
}

String design="christmas";


void loopWifi() {
    if (WiFiMulti.run() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      //Serial.print("[HTTP] begin...\n");
      if (http.begin(client, api)) {  // HTTP
        //Serial.print("[HTTP] GET...\n");
        int httpCode = http.GET();
        if (httpCode > 0) {
          //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String get=http.getString();
            design=get.substring(20,get.length());
            datetime=get.substring(0,19);
            Serial.println(datetime);
            Serial.println(design);
            //Serial.printf("[HTTP] GET: %d\n", design);
          }
        } else {
          //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        //Serial.printf("[HTTP] Unable to connect\n");
      }
    }
  //delay(1000);
}

int position=0;

/**********************************************************************************************/
#include <Wire.h>

const int MPU_ADDR =      0x68; // definição do endereço do sensor MPU6050 (0x68)
const int WHO_AM_I =      0x75; // registro de identificação do dispositivo
const int PWR_MGMT_1 =    0x6B; // registro de configuração do gerenciamento de energia
const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int ACCEL_XOUT =    0x3B; // registro de leitura do eixo X do acelerômetro
 
const int sda_pin = D5; // definição do pino I2C SDA
const int scl_pin = D6; // definição do pino I2C SCL
 
// variáveis para armazenar os dados "crus" do acelerômetro
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 

void writeRegMPU(int reg, int val) {
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.write(val);                      // escreve o valor no registro
  Wire.endTransmission(true);           // termina a transmissão
}

uint8_t readRegMPU(uint8_t reg) {
  uint8_t data;
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 1);        // configura para receber 1 byte do registro escolhido acima
  data = Wire.read();                   // lê o byte e guarda em 'data'
  return data;                          //retorna 'data'
}

void findMPU(int mpu_addr) {
  Wire.beginTransmission(MPU_ADDR);
  int data = Wire.endTransmission(true);
  if(data == 0) {
    Serial.print("Dispositivo encontrado no endereço: 0x");
    Serial.println(MPU_ADDR, HEX);
  } else {
    Serial.println("Dispositivo não encontrado!");
  }
}
 
void checkMPU(int mpu_addr) {
  findMPU(MPU_ADDR);
  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
  if(data == 104)  {
    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
    data = readRegMPU(PWR_MGMT_1);
    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
    else Serial.println("MPU6050 em modo ACTIVE!"); 
  }
  else Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
}
 
void initMPU() {
  writeRegMPU(PWR_MGMT_1, 0); // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
  writeRegMPU(GYRO_CONFIG, 0);
  writeRegMPU(ACCEL_CONFIG, 0);
}
 
void readRawMPU() {  
  Wire.beginTransmission(MPU_ADDR);       // inicia comunicação com endereço do MPU6050
  Wire.write(ACCEL_XOUT);                       // envia o registro com o qual se deseja trabalhar, começando com registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);            // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 14);         // configura para receber 14 bytes começando do registro escolhido acima (0x3B)
 
  AcX = Wire.read() << 8;                 // lê primeiro o byte mais significativo
  AcX |= Wire.read();                     // depois lê o bit menos significativo
  AcY = Wire.read() << 8;
  AcY |= Wire.read();
  AcZ = Wire.read() << 8;
  AcZ |= Wire.read();
 
  Tmp = Wire.read() << 8;
  Tmp |= Wire.read();
 
  GyX = Wire.read() << 8;
  GyX |= Wire.read();
  GyY = Wire.read() << 8;
  GyY |= Wire.read();
  GyZ = Wire.read() << 8;
  GyZ |= Wire.read(); 
 
 /*
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  */
  

  //if (AcY>-500 && AcY<500) {
    if (AcX>14000) {
      //quadro tá em pé
      position=0;
    } else if (AcX<-14000) {
      //quadro de ponta cabeça
      position=180;
    } else 
  //} else if (AcX>-500 && AcX<500) {
    if (AcY>14000) {
      position=-90;
    } else if (AcY<-14000) {
      position=90;
    //}
  } else {
    //position=0;
  }

  //acx
  //up    =  16000 ~  17000
  //down  = -15000 ~ -17000
  //acy =      0 ~ 500



  //acy
  //left  = -16000 ~ 17000
  //right =  16000 ~ 17000
  //acx =      0 ~ 500

  //delay(50);                                        
}

void setupGyro() {
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("nIniciando configuração do MPU6050n");
  Wire.begin(sda_pin, scl_pin);
  initMPU();
  checkMPU(MPU_ADDR);
  Serial.println("nConfiguração finalizada, iniciando loopn");  
}

void loopGyro() {
  readRawMPU();    // lê os dados do sensor
  //delay(100);  
}
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
  //FastLED.setBrightness(96);
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

/*
int Alphabet[16][12]={
  {1,1,1,0,1,1,0,0,1,1,1,0,1,1,0},
  {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1},
  {1,1,1,0,1,1,0,0,1,0,0,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,0,0,0,1,0,1},
  {1,0,1,0,1,1,0,0,1,1,1,0,1,1,0}
};
*/


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


//pikachu
int Pikachu[32][32]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,1,1,0},
    {0,0,1,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,1,0,0},
    {0,0,0,1,2,2,2,2,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,0,0,0},
    {0,0,0,0,0,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,0,0,0,0},
    {0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,1,2,2,1,2,2,2,2,2,1,2,2,1,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,3,3,2,2,2,2,2,2,2,2,2,2,2,3,3,2,2,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,3,3,2,2,2,2,2,2,2,2,2,2,2,3,3,2,2,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,2,2,2,2,2,3,3,3,3,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,3,3,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void drawPikachu() {
  //white();
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      if (Pikachu[y-1][x-1]>0) {
        String c="#000000";

        if (Pikachu[y-1][x-1]==1) {
          c="#222222"; //pontas das orelhas
        } else if (Pikachu[y-1][x-1]==2) {
          c="#DDDD00"; //face
        } else if (Pikachu[y-1][x-1]==3) {
          c="#DA0000"; //bochecha
        }

        //o 10 é deslocamento
        p(x,32-y,c);
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
  /*
  if (effect==0) {
    randPixels();
  } else if (effect==1) {
    tree();
    rain();
  } else if (effect==2) {
    tree();
    snow();
  } else if (effect==3) {
    draw();
  } else if (effect==4) {
    ball();
  } else if (effect==5) {
    bars();
  } else {
    randPixels();
  }
  */
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

/*
  int r=random(1,10);
  if (r==1) {
    return "#FF0000";
  } else if (r==2) {
    return "#FFFF00";
  } else if (r==3) {
    return "#00FFFF";
  } else if (r==4) {
    return "#0000FF";
  } else if (r==5) {
    return "#00FF00";
  } else if (r==6) {
    return "#FF00FF";
  } else if (r==7) {
    return "#FFCCFF";
  } else if (r==8) {
    return "#FFCCCC";
  } else if (r==9) {
    return "#CCFFCC";
  } else if (r==10) {
    return "#CCCCFF";
  }
  */
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

int lunch[32][32]={
  {0,0,0,0,0,0,0,0,0,1,2,0,0},
  {0,0,0,0,0,0,0,0,4,3,1,2,0},
  {0,0,0,0,0,0,0,4,4,3,3,1,2},
  {0,0,0,0,0,0,4,4,4,4,3,1,2},
  {0,0,0,0,0,4,4,5,4,4,3,1,2},
  {0,0,0,0,4,4,4,4,4,4,3,1,2},
  {0,0,0,4,4,5,4,4,4,4,3,1,2},
  {0,0,4,4,4,4,4,5,4,4,3,1,2},
  {0,4,4,4,5,4,4,4,4,3,1,2,0},
  {1,3,3,4,4,4,4,3,3,1,2,2,0},
  {2,1,3,3,3,3,3,3,1,2,2,0,0},
  {0,2,1,1,1,1,1,1,1,2,0,0,0},
  {0,0,0,2,2,2,2,2,0,0,0,0,0}
};

void drawLunch() {
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      if (lunch[y-1][x-1]>0) {
        String c="#000000";

        if (lunch[y-1][x-1]==2) {
          c="#00FF00";
        } else if (lunch[y-1][x-1]==1) {
          c="#33CC33";
        } else if (lunch[y-1][x-1]==3) {
          c="#DA6633";
        } else if (lunch[y-1][x-1]==4) {
          c="#DA0000";
        } else if (lunch[y-1][x-1]==5) {
          c="#333333";
        }

        //o 10 é deslocamento
        p(x+10,(13-y)+12,c);
      }
    }
  }
}

/*
int matrix[32][32]={
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
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

*/
int clockArray[33][33]={
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

/*
void drawClock() {
  for(int x=1;x<=32;x++) {
    for(int y=1;y<=32;y++) {
      if (clockArray[y-1][x-1]>0) {
        String c="#000000";

        if (clockArray[y-1][x-1]==1) {
          c="#999999";
        } else if (clockArray[y-1][x-1]==2) {
          c="#33CC33";
        }

        p(x,33-y,c);
      }
    }
  }
}
*/

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
  //if (bool_square) {
  //  count_square++;
  //} else {
    count_square--;
  //}
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


void twinkleTwinkle() {
  //brilha brilha estrelinha

}







int clock1234[33][33]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,6,10,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0}
};


int clock5678[33][33]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,6,10,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}
};


int clock90[33][33]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,6,10,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}
};

String colorRand=randColor();
void drawClock(int m[][33], int p_, int xx_, int yy_) {
  int x_=0;
  int y_=0;
  if (p_==0) {
    x_=0;
    y_=0;
  } else if (p_==1) {
    x_=16;
    y_=0;
  } else if (p_==2) {
    x_=0;
    y_=16;
  } else if (p_==3) {
    x_=16;
    y_=16;
  }
  //String c=randColor();

  //c="#666666";

  for(int x=1;x<=16;x++) {
    for(int y=1;y<=16;y++) {
      if (m[y+y_-1][x+x_-1]==1) {
        p(x+xx_,33-(y+yy_),colorRand);
      }
    }
  }
}



void drawClock_(int n, int x_, int y_) {
  //int n=1;
  //int x_=0;
  //int y_=0;
  if (n==1) {
      drawClock(clock1234,0,0+x_,0+y_);
    } else if (n==2) {
      drawClock(clock1234,1,0+x_,0+y_);
    } else if (n==3) {
      drawClock(clock1234,2,0+x_,0+y_);
    } else if (n==4) {
      drawClock(clock1234,3,0+x_,0+y_);
    } else if (n==5) {
      drawClock(clock5678,0,0+x_,0+y_);
    } else if (n==6) {
      drawClock(clock5678,1,0+x_,0+y_);
    } else if (n==7) {
      drawClock(clock5678,2,0+x_,0+y_);
    } else if (n==8) {
      drawClock(clock5678,3,0+x_,0+y_);
    } else if (n==9) {
      drawClock(clock90,0,0+x_,0+y_);
    } else if (n==0) {
      drawClock(clock90,3,0+x_,0+y_);
    }
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
  unsigned long currentMillisClock = millis();
  if (previousMillisClock==0 || currentMillisClock - previousMillisClock >= intervalClock) {
    colorRand=randColor();
    previousMillisClock = currentMillisClock;
    int y_=datetime.substring(0,4).toInt();
    int m_=datetime.substring(5,7).toInt();
    int d_=datetime.substring(8,10).toInt();

    int h_=datetime.substring(11,13).toInt();
    int i_=datetime.substring(14,16).toInt();
    int s_=datetime.substring(17,19).toInt();

    s_++;
    if (s_==60) {
      s_=0;
      i_++;
      if (i_==60) {
        i_=0;
        h_++;
        if (h_==24) {
          h_=0;
        }
      }
    }

    String ss_=String(s_);
    if (s_<10) {
      ss_="0"+String(s_);
    }

    String ii_=String(i_);
    if (i_<10) {
      ii_="0"+String(i_);
    }

    String hh_=String(h_);
    if (h_<10) {
      hh_="0"+String(h_);
    }

    String dd_=String(d_);
    if (d_<10) {
      dd_="0"+String(d_);
    }

    String mm_=String(m_);
    if (m_<10) {
      mm_="0"+String(m_);
    }


    

    datetime=String(y_)+"-"+mm_+"-"+dd_+" "+hh_+":"+ii_+":"+ss_;
    Serial.println(datetime);
  }
  unsigned long currentMillis = millis();
  if (previousMillis==0 || currentMillis - previousMillis >= interval) {
    colorRand=randColor();
    loopWifi();
    //toggle();
    previousMillis = currentMillis;
    effect++;
    if (effect>6) {
      effect=0;
    }
  }
  //white();
  black();
  //toggle();


  //design="clock";

  if (design=="christmas") {
    //position=180;
    border();
    tree();
    snow();
  } else if (design=="mario") {
    //position=180;
    draw();
  } else if (design=="random") {
    //position=180;
    randPixels();
  } else if (design=="rainbow") {
    rainbow();
  } else if (design=="ball") {
    //position=180;
    ball();
  } else if (design=="fall") {
    //position=180;
    fall();
  } else if (design=="bars") {
    //position=180;
    bars();
  } else if (design=="lunch") {
    //position=180;
    squareEffect();
    drawLunch();
  } else if (design=="square") {
    //position=180;
    squareEffect();
  } else if (design=="pikachu") {
    //position=180;
    drawPikachu();
  } else if (design=="clock") {


    //datetime
    String y=datetime.substring(0,4);
    String m=datetime.substring(5,7);
    String d=datetime.substring(8,10);

    String h=datetime.substring(11,13);
    String i=datetime.substring(14,16);
    String s=datetime.substring(17,19);

    int h0=h.substring(0,1).toInt();
    int h1=h.substring(1,2).toInt();

    int m0=i.substring(0,1).toInt();
    int m1=i.substring(1,2).toInt();

    int s0=s.substring(0,1).toInt();
    int s1=s.substring(1,2).toInt();

    

/*
    Serial.println(y);
    Serial.println(m);
    Serial.println(d);
    Serial.println(h);
    Serial.println(i);
    Serial.println(s);

    Serial.println("------------");
    Serial.println(h0);
    Serial.println(h1);
    Serial.println(m0);
    Serial.println(m1);
    */
    all("#000022");

    drawClock_(h0,0,0);
    drawClock_(h1,16,0);
    drawClock_(m0,0,16);
    drawClock_(m1,16,16);

    //for(int x=1;x<(s.toInt()/2);x++) {
    //  p(x,1,"#712580");
    //  p(33-x,1,"#712580");
    //}

    //efeito de pixels em volta, de acordo com os segundos
    for(int x=1;x<=map(s.toInt(),0,59,1,32);x++) {
      p(x,32,randColor());
      p(33-x,1,randColor());

      p(1,x,randColor());
      p(32,33-x,randColor());
    }

    snow();
    //rain();
/*
    drawClock(clock5678,3,0,0);
    drawClock(clock1234,2,0,16);
    drawClock(clock1234,1,16,0);
    drawClock(clock1234,0,16,16);
    */

  } else if (design=="clocks") {
    //position=180;
    //drawClock();

    lineX(16,1,32,"#666666");
    lineX(17,1,32,"#666666");

    lineY(16,1,32,"#666666");
    lineY(17,1,32,"#666666");

    square(5,28,5,28,"#DD0000");
    square(10,23,10,23,"#DD0000");

  }

 
  //draw();
  //snow();
  //border();
  //tree();
  //draw();
  //snow();
  //FastLED.delay(1);
  FastLED.show();
}
