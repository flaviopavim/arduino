#include <Servo.h> 

//postas do Arduino
//-----------------------------------------
int pin_servo_1 = A0;
int pin_servo_2 = A1;

#define CH1 3
#define CH2 5
#define CH3 6
//-----------------------------------------

Servo servo_1,servo_2; 

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup(){
  Serial.begin(115200);
   pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  servo_1.attach(pin_servo_1);
  servo_2.attach(pin_servo_2);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int angle_servo_1=80;
int angle_servo_2=100;

void loop() {
  //faz leitura do Rádio Controle
  int ch1Value = readChannel(CH1, -100, 100, -100);
  int ch2Value = readChannel(CH2, -100, 100, 0);
  int ch3Value = readChannel(CH3, -100, 100, 0);
  
  //printa os valores no serial monitor
  Serial.print(" CH2: ");
  Serial.print(ch2Value);
  Serial.print(" CH3: ");
  Serial.println(ch3Value);

  //mapeia o valor do servo proporcional ao que vem do rádio controle
  double aileron=map(ch2Value,-120,120,-45,45);
  double elevator=map(ch3Value,-120,120,-45,45);

  //move os servos
  servo_1.write(angle_servo_1+elevator-aileron);   
  servo_2.write(angle_servo_2-elevator-aileron);

}