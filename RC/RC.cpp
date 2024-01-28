//Define all ports pinned from radio control receptor

#define CH1 3
#define CH2 5
#define CH3 6
#define CH4 9
#define CH5 10

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup(){

  Serial.begin(115200);

  //Define ports with input
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);

}

//Variables to set radio signals
int ch1Value, ch2Value, ch3Value, ch4Value; //Analog buttons
bool ch5Value;  //Switch

void loop() {

  //Read all channels and switches
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, -100);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch5Value = readSwitch(CH5, false);

  //Print all channels
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" Ch4: ");
  Serial.print(ch4Value);
  Serial.print(" Ch5: ");
  Serial.println(ch5Value);

  //Pause
  delay(500);

}