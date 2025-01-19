// Portas e ligações
const int buzzer1 = 3;
const int buzzer2 = 2;

// Inicialização do Arduino
void setup() {
  pinMode(buzzer1, OUTPUT); // Inicializar o primeiro buzzer
  pinMode(buzzer2, OUTPUT); // Inicializar o segundo buzzer
}

// Variáveis do loop
unsigned long previousMillis = 0;
int interval = 1;

int countInterval1On = 0;
int countInterval1Off = 0;

int countInterval2On = 0;
int countInterval2Off = 0;

int intervalBuzzer1On = 50;
int intervalBuzzer1Off = 50;

int intervalBuzzer2On = 50;
int intervalBuzzer2Off = 50;

bool boolTone1=false;
bool boolTone2=false;

int countNote1=0;
int countNote2=0;

int notesee[] = {
  523, 587, 659, 698, // Escala de dó
  587, 659, 698, 783, // Escala de ré
  659, 698, 783, 880, // Escala de mi
  698, 783, 880, 987, // Escala de fá
  //783, 880, 987, 1046, // Escala de sol
  //880, 987, 1046, 1174, // Escala de lá
  //987, 1046, 1174, 1318, // Escala de si
};

int notes[] = {
  //523
  523, 659, 783
};

// Número de amostras para a média móvel
const int numSamples = 10;

// Array para armazenar as amostras
int samples[numSamples];

// Índice da próxima amostra
int nextSampleIndex = 0;

// Soma das amostras
int samplesSum = 0;

int smoothedValue=100;

void loopButton() {
  // Lê o valor do sensor
  int sensorValue = analogRead(A4);

  // Subtrai o valor da amostra mais antiga
  samplesSum -= samples[nextSampleIndex];

  // Adiciona o novo valor à soma
  samplesSum += sensorValue;

  // Armazena o novo valor no array
  samples[nextSampleIndex] = sensorValue;

  // Incrementa o índice para a próxima amostra
  nextSampleIndex = (nextSampleIndex + 1) % numSamples;

  // Calcula a média das amostras
  smoothedValue = samplesSum / numSamples;

}

bool tone1=false;

bool button=false;
void playBuzzer1() {
  if (!boolTone1) {

    countInterval1On++;

    if (countInterval1On >= intervalBuzzer1On) {
      countInterval1On = 0;

      int note1=notes[countNote1]+map(analogRead(A0),0,1024,-600,600);
      
      countNote1++;
      int length1 = sizeof(notes) / sizeof(notes[0]);
      if (countNote1>=length1) {
        countNote1=0;
      }

      tone(buzzer1, note1);

      boolTone1=true;
      countInterval1Off = 0;
    }
  }

  if (boolTone1) {
    countInterval1Off++;
  }

  if (countInterval1Off >= intervalBuzzer1Off) {
    countInterval1Off = 0;
    //if (button) {
    noTone(buzzer1);
    //}
    boolTone1=false;
  }
}


void playBuzzer2() {
  if (!boolTone2) {

    countInterval2On++;

    if (countInterval2On >= intervalBuzzer2On) {
      countInterval2On = 0;

      int note2=notes[countNote2]+map(analogRead(A1),0,1024,-300,300);
      countNote2++;
      int length2 = sizeof(notes) / sizeof(notes[0]);
      if (countNote2>=length2) {
        countNote2=0;
      }

      tone(buzzer2, note2);

      boolTone2=true;
      countInterval2Off = 0;
    }
  }

  if (boolTone2) {
    countInterval2Off++;
  }

  if (countInterval2Off >= intervalBuzzer2Off) {
    countInterval2Off = 0;
    noTone(buzzer2);
    boolTone2=false;
  }
}



// Magic ;)
void loop() {

  loopButton();
  if (smoothedValue < 10) {
    button=true;
  } else {
    button=false;
  }

  //interval=constrain(map(analogRead(A3),0,1024,1,10),1,10);

  int value=map(analogRead(A3),0,1024,1,1200);

  intervalBuzzer1On=value*2;
  intervalBuzzer1Off=value;
  intervalBuzzer2On=value*2;
  intervalBuzzer2Off=value;

  //countInterval1Off=map(analogRead(A4),0,1024,1,300);
  //countInterval2Off=map(analogRead(A4),0,1024,1,300);

/*
  if (smoothedValue < 1) {
    notes[0] = 783;
    notes[1] = 880;
    notes[2] = 987;
    notes[3] = 1046;
  } else {
      notes[0] = 523;
      notes[1] = 587;
      notes[2] = 659;
      notes[3] = 698;
  }
  */

  // Busca em qual milissegundo está
  unsigned long currentMillis = millis();
  
  // Se a contagem de milissegundos for igual ao intervalo
  if (currentMillis - previousMillis >= interval) {
    // Seta o tempo atual na variável
    previousMillis = currentMillis;

    playBuzzer1();
    //delay(10);
    //playBuzzer2();
    //delay(10);

  }
}