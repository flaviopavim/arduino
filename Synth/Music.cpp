#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

// Portas e ligações
const int buzzer = 3;

// Inicialização do Arduino
void setup() {
  pinMode(buzzer, OUTPUT); // Inicializar o primeiro buzzer
}

// Variáveis do loop
unsigned long previousMillis = 0;
int interval = 1;

int countIntervalOn = 0;
int countIntervalOff = 0;

int intervalBuzzerOn = 50;
int intervalBuzzerOff = 50;

bool boolTone = false;

int countNote = 0;

int melodyw[][2] = {
  {523, 500}, // Dó
  {659, 500}, // Mi
  {783, 500}  // Sol
  // Adicione mais linhas conforme necessário para mais melodias
};

int melodsy[][2] = {
  
  {NOTE_E5, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_C5, 500}, {NOTE_B4, 500},
  {NOTE_A4, 2000}, {NOTE_A4, 500}, {NOTE_C5, 500}, 
  {NOTE_E5, 2000}, {NOTE_D5, 500}, {NOTE_C5, 500},
  {NOTE_B4, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_E5, 2000}, {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000},
  
  {NOTE_D5, 2000}, {NOTE_D5, 500}, {NOTE_F5, 500}, 
  {NOTE_A5, 2000}, {NOTE_G5, 500}, {NOTE_F5, 500},
  {NOTE_E5, 2000}, {NOTE_E5, 500}, {NOTE_C5, 500}, 
  {NOTE_E5, 2000}, {NOTE_D5, 500}, {NOTE_C5, 500}, 
  {NOTE_B4, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_E5, 2000}, {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000},

  
  {NOTE_E5, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_C5, 500}, {NOTE_B4, 500},
  {NOTE_A4, 2000}, {NOTE_A4, 500}, {NOTE_C5, 500}, 
  {NOTE_E5, 2000}, {NOTE_D5, 500}, {NOTE_C5, 500},
  {NOTE_B4, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_E5, 2000}, {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000},
  
  {NOTE_D5, 2000}, {NOTE_D5, 500}, {NOTE_F5, 500}, 
  {NOTE_A5, 2000}, {NOTE_G5, 500}, {NOTE_F5, 500},
  {NOTE_E5, 2000}, {NOTE_E5, 500}, {NOTE_C5, 500}, 
  {NOTE_E5, 2000}, {NOTE_D5, 500}, {NOTE_C5, 500}, 
  {NOTE_B4, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, 
  {NOTE_D5, 2000}, {NOTE_E5, 2000}, {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000},


  {NOTE_E5, 6000}, {NOTE_C5, 6000}, {NOTE_D5, 6000}, {NOTE_B4, 6000}, {NOTE_C5, 6000}, {NOTE_A4, 6000},  {NOTE_GS4, 14000}, 
  /*
  {NOTE_E5, 6000}, {NOTE_C5, 6000}, {NOTE_D5, 6000}, {NOTE_B4, 6000}, {NOTE_C5, 6000}, {NOTE_D5, 6000},  {NOTE_GS4, 12000}, 
  */
  
};

int melodsdy[][2] = {
  {NOTE_E5, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, {NOTE_D5, 2000}, {NOTE_C5, 500}, {NOTE_B4, 500},
  {NOTE_A4, 2000}, {NOTE_A4, 500}, {NOTE_C5, 500}, {NOTE_E5, 2000}, {NOTE_D5, 500}, {NOTE_C5, 500},
  {NOTE_B4, 2000}, {NOTE_B4, 500}, {NOTE_C5, 500}, {NOTE_D5, 2000}, {NOTE_E5, 2000},
  {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000}, {0, 2000}, 
  {NOTE_B4, 2000}, {NOTE_C5, 2000}, {NOTE_D5, 2000}, {0, 2000},
  {NOTE_D5, 2000}, {NOTE_F5, 1000}, {NOTE_A5, 1000}, {NOTE_G5, 2000}, {NOTE_F5, 1000},
  {NOTE_E5, 4000}, {NOTE_C5, 1000}, {NOTE_E5, 1000}, {NOTE_D5, 2000}, {NOTE_C5, 1000},
  {NOTE_B4, 2000}, {NOTE_B4, 1000}, {NOTE_C5, 1000}, {NOTE_D5, 2000}, {NOTE_E5, 2000},
  {NOTE_C5, 2000}, {NOTE_A4, 2000}, {NOTE_A4, 6000}, {0, 2000}
};

int melody[][2] = {
  
  { NOTE_E5, 500 }, { NOTE_E5, 500 }, { REST, 500 }, { NOTE_E5, 500 }, { REST, 500 }, { NOTE_C5, 500 }, { NOTE_E5, 500 },
  { NOTE_G5, 1000 }, { REST, 1000 }, { NOTE_G4, 500 }, { REST, 500 },
  { NOTE_C5, 250 }, { NOTE_G4, 500 }, { REST, 250 }, { NOTE_E4, 250 },
  { NOTE_A4, 500 }, { NOTE_B4, 500 }, { NOTE_AS4, 500 }, { NOTE_A4, 500 },
  { NOTE_G4, 1000 }, { NOTE_E5, 1000 }, { NOTE_G5, 1000 }, { NOTE_A5, 1000 }, { NOTE_F5, 500 }, { NOTE_G5, 500 },
  { REST, 500 }, { NOTE_E5, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 500 }, { NOTE_B4, 250 },
  { NOTE_C5, 250 }, { NOTE_G4, 500 }, { REST, 250 }, { NOTE_E4, 250 },
  { NOTE_A4, 500 }, { NOTE_B4, 500 }, { NOTE_AS4, 500 }, { NOTE_A4, 500 },
  { NOTE_G4, 1000 }, { NOTE_E5, 1000 }, { NOTE_G5, 1000 }, { NOTE_A5, 1000 }, { NOTE_F5, 500 }, { NOTE_G5, 500 },
  { REST, 500 }, { NOTE_E5, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 500 }, { NOTE_B4, 250 },
  { NOTE_C5, 250 }, { NOTE_G4, 500 }, { REST, 250 }, { NOTE_E4, 250 },
  { NOTE_A4, 500 }, { NOTE_B4, 500 }, { NOTE_AS4, 500 }, { NOTE_A4, 500 },
  { NOTE_G4, 1000 }, { NOTE_E5, 1000 }, { NOTE_G5, 1000 }, { NOTE_A5, 1000 }, { NOTE_F5, 500 }, { NOTE_G5, 500 },
  
  { REST, 500 }, { NOTE_G5, 500 }, { NOTE_FS5, 500 }, { NOTE_F5, 500 }, { NOTE_DS5, 500 }, { NOTE_E5, 500 },
  { REST, 500 }, { NOTE_GS4, 500 }, { NOTE_A4, 500 }, { NOTE_C4, 500 }, { REST, 500 }, { NOTE_A4, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 500 },
  { REST, 500 }, { NOTE_DS5, 500 }, { REST, 500 }, { NOTE_D5, 250 },
  { NOTE_C5, 250 }, { REST, 250 },
  
  { REST, 500 }, { NOTE_G5, 500 }, { NOTE_FS5, 500 }, { NOTE_F5, 500 }, { NOTE_DS5, 500 }, { NOTE_E5, 500 },
  { REST, 500 }, { NOTE_GS4, 500 }, { NOTE_A4, 500 }, { NOTE_C4, 500 }, { REST, 500 }, { NOTE_A4, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 500 },
  { REST, 500 }, { NOTE_DS5, 500 }, { REST, 500 }, { NOTE_D5, 250 },
  { NOTE_C5, 250 }, { REST, 250 },
  
  { NOTE_C5, 500 }, { NOTE_C5, 250 }, { NOTE_C5, 500 }, { REST, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 250 },
  { NOTE_E5, 500 }, { NOTE_C5, 250 }, { NOTE_A4, 500 }, { NOTE_G4, 1000 },
  
  { NOTE_C5, 500 }, { NOTE_C5, 250 }, { NOTE_C5, 500 }, { REST, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 500 }, { NOTE_E5, 500 },
  { REST, 1000 },
  
  { NOTE_C5, 500 }, { NOTE_C5, 250 }, { NOTE_C5, 500 }, { REST, 500 }, { NOTE_C5, 500 }, { NOTE_D5, 250 },
  { NOTE_E5, 500 }, { NOTE_C5, 250 }, { NOTE_A4, 500 }, { NOTE_G4, 1000 },
  
  { NOTE_E5, 500 }, { NOTE_C5, 250 }, { NOTE_G4, 500 }, { REST, 500 }, { NOTE_GS4, 500 },
  { NOTE_A4, 500 }, { NOTE_F5, 500 }, { NOTE_F5, 500 }, { NOTE_A4, 1000 },
  { NOTE_D5, 250 }, { NOTE_A5, 250 }, { NOTE_A5, 250 }, { NOTE_A5, 250 }, { NOTE_G5, 250 }, { NOTE_F5, 250 },
  
  { NOTE_E5, 500 }, { NOTE_C5, 250 }, { NOTE_A4, 500 }, { NOTE_G4, 1000 },
  { NOTE_E5, 500 }, { NOTE_C5, 250 }, { NOTE_G4, 500 }, { REST, 500 }, { NOTE_GS4, 500 },
  { NOTE_A4, 500 }, { NOTE_F5, 500 }, { NOTE_F5, 500 }, { NOTE_A4, 1000 },
  { NOTE_B4, 500 }, { NOTE_F5, 500 }, { NOTE_F5, 500 }, { REST, 500 }, { NOTE_E5, 500 }, { NOTE_D5, 500 },
  { NOTE_C5, 500 }, { NOTE_E4, 250 }, { NOTE_E4, 500 }, { NOTE_C4, 1000 },
  
  //game over sound
  { NOTE_C5, 250 }, { NOTE_G4, 250 }, { NOTE_E4, 500 },
  { NOTE_A4, 125 }, { NOTE_B4, 125 }, { NOTE_A4, 125 }, { NOTE_GS4, 125 }, { NOTE_AS4, 125 }, { NOTE_GS4, 125 },
  { NOTE_G4, 250 }, { NOTE_D4, 250 }, { NOTE_E4, 500 },  

};



void playBuzzer() {
  if (!boolTone) {
    countIntervalOn++;

    if (countIntervalOn >= intervalBuzzerOn) {
      countIntervalOn = 0;

      int note = melody[countNote][0];
      //int duration = melody[countNote][1]/5;
      int duration = (melody[countNote][1]/5)*map(analogRead(A0),0,1024,1,5);

    
      intervalBuzzerOff=duration;
      
      countNote++;
      int length = sizeof(melody) / sizeof(melody[0]);
      if (countNote >= length) {
        countNote = 0;
      }

      tone(buzzer, note*map(analogRead(A3),100,1024,1,5), duration);
      //tone(buzzer, note, duration);
      boolTone = true;
      countIntervalOff = 0;
    }
  }

  if (boolTone) {
    countIntervalOff++;
  }

  if (countIntervalOff >= intervalBuzzerOff) {
    countIntervalOff = 0;
    noTone(buzzer);
    boolTone = false;
  }
}

// Magic ;)
void loop() {


  //intervalBuzzerOn=map(analogRead(A1),0,1024,1,1000);

  // Busca em qual milissegundo está
  unsigned long currentMillis = millis();
  
  // Se a contagem de milissegundos for igual ao intervalo
  if (currentMillis - previousMillis >= interval) {
    // Seta o tempo atual na variável
    previousMillis = currentMillis;

    playBuzzer();
  }
}
