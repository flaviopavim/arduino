#include <Servo.h>
#include <EnableInterrupt.h>
Servo motor,ailerons,profundor,leme; // create servo object to control the ESC

//controle de 3 servos: aileron, aprofundor e leme
//controle do motor
//entrada 4 portas pwm ()

//pwm: 3,5,6,9,10,11

//portas Arduino Pro Mini
#define P_MOTOR 3 
#define SERVO_AILERONS 9
#define SERVO_PROFUNDOR 8  
#define SERVO_LEME 7
#define RC_CH1_INPUT  6
#define RC_CH2_INPUT  5
#define RC_CH3_INPUT  4
#define RC_CH4_INPUT  2
//#define RC_CH1_INPUT  A0
//#define RC_CH2_INPUT  A1
//#define RC_CH3_INPUT  A2
//#define RC_CH4_INPUT  A3

//Não mexer daqui pra baixo
#define RC_NUM_CHANNELS  4
#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
    noInterrupts();
    memcpy(rc_values, (const void *) rc_shared, sizeof (rc_shared));
    interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
    if (digitalRead(input_pin) == HIGH) {
        rc_start[channel] = micros();
    } else {
        uint16_t rc_compare = (uint16_t) (micros() - rc_start[channel]);
        rc_shared[channel] = rc_compare;
    }
}

void calc_ch1() {
    calc_input(RC_CH1, RC_CH1_INPUT);
}

void calc_ch2() {
    calc_input(RC_CH2, RC_CH2_INPUT);
}

void calc_ch3() {
    calc_input(RC_CH3, RC_CH3_INPUT);
}

void calc_ch4() {
    calc_input(RC_CH4, RC_CH4_INPUT);
}

void setup() {
    Serial.begin(9600);
    pinMode(RC_CH1_INPUT, INPUT);
    pinMode(RC_CH2_INPUT, INPUT);
    pinMode(RC_CH3_INPUT, INPUT);
    pinMode(RC_CH4_INPUT, INPUT);
    enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
    enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
    enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
    enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
    motor.attach(P_MOTOR); // (pin, min pulse width, max pulse width in microseconds) 
//    motor.attach(P_MOTOR, 1000, 2000); // (pin, min pulse width, max pulse width in microseconds) 
    ailerons.attach(SERVO_AILERONS);
    profundor.attach(SERVO_PROFUNDOR);
    leme.attach(SERVO_LEME);
}

int min_ch1 = 0,max_ch1 = 2000;
int min_ch2 = 0,max_ch2 = 2000;
int min_ch3 = 0,max_ch3 = 2000;
int min_ch4 = 0,max_ch4 = 2000;

bool radio=true; //ta errado issaqui

void loop() {
    rc_read_values();

    int v_acceleration;
    int v_ailerons;
    int v_profundor;
    int v_leme;
    if (radio) {
        v_acceleration = map(rc_values[RC_CH1], min_ch1, max_ch1, 90, 130);
        v_ailerons = map(rc_values[RC_CH1], min_ch2, max_ch2, -180, 180);
        v_profundor = map(rc_values[RC_CH1], min_ch3, max_ch3, -180, 180);
        v_leme = map(rc_values[RC_CH1], min_ch4, max_ch4, -180, 180);
    } else {
        //se tiver sem sinal de radio, fudeu manolo!!
    }

    motor.write(v_acceleration);
    ailerons.write(v_ailerons);
    profundor.write(v_profundor);
    leme.write(v_leme);

    Serial.print("CH1:");
    Serial.print(rc_values[RC_CH1]);
    Serial.print("\t");
    Serial.print("CH2:");
    Serial.print(rc_values[RC_CH2]);
    Serial.print("\t");
    Serial.print("CH3:");
    Serial.print(rc_values[RC_CH3]);
    Serial.print("\t");
    Serial.print("CH4:");
    Serial.println(rc_values[RC_CH4]);
}