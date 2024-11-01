#include <Servo.h> 

int servopin1 = A0;
int servopin2 = A1;
int servopin3 = A2;
int servopin4 = A3;
int servopin5 = 13;
 
Servo servo1;  
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
 
int angle = 90;   // servo position in degrees 

int correction_front_left=-5;
int correction_front_right=-15;
int correction_rear_right=-10;
int correction_rear_left=5;
int correction_axis=-8;

void frontLeft(int angle_) {
  servo1.write(angle-angle_+correction_front_left); //front left       
}
void frontRight(int angle_) {
  servo2.write(angle+angle_+correction_front_right); //front right
}
void rearRight(int angle_) {
  servo3.write(angle+angle_+correction_rear_right); //rear right       
}
void rearLeft(int angle_) {
  servo4.write(angle-angle_+correction_rear_left); //rear left
}
void axis(int angle_) {
  servo5.write(angle+angle_+correction_axis);
}

void stand() {
  axis(0);
  frontLeft(0);
  frontRight(0);
  rearLeft(0);
  rearRight(0);
}

void stay() {
  axis(0);
  frontLeft(30);
  frontRight(30);
  rearLeft(-30);
  rearRight(-30);
}
 
void setup() { 
  servo1.attach(servopin1); 
  servo2.attach(servopin2); 
  servo3.attach(servopin3); 
  servo4.attach(servopin4); 
  servo5.attach(servopin5); 
  stay();
} 
 
void dancing() {
  for(angle = 90-15; angle < 90+30; angle++) {
    servo1.write(angle+correction_front_left); //front left          
    servo2.write(angle+correction_front_right); //front right
    servo3.write(angle+correction_rear_right); //rear right        
    servo4.write(angle+correction_rear_left); //rear left
    delay(10);
  }
  for(angle = 90+15; angle > 90-30; angle--) {
    servo1.write(angle+correction_front_left); //front left          
    servo2.write(angle+correction_front_right); //front right
    servo3.write(angle+correction_rear_right); //rear right        
    servo4.write(angle+correction_rear_left); //rear left
    delay(10);
  }
}

void maizein() {
  for(int i = -20; i < 20; i++) {
    frontLeft(i);
    rearRight(i);

    frontRight(i);
    rearLeft(i);
    delay(20);
  }

  for(int i = 20; i > -20; i--) {
    frontLeft(i);
    rearRight(i);

    frontRight(i);
    rearLeft(i);
    delay(20);
  }
}


void walk2() {
  for(int i = -20; i < 20; i++) {
    frontLeft(i);
    rearRight(i);

    frontRight(i);
    rearLeft(i);
    delay(20);
  }

  for(int i = 20; i > -20; i--) {
    frontLeft(i);
    rearRight(i);

    frontRight(i);
    rearLeft(i);
    delay(20);
  }


  
}


void doubleLegs() {
    
}

bool bool_front_left=false;
bool bool_front_right=true;
bool bool_rear_left=false;
bool bool_rear_right=true;
bool bool_axis=false;

int maximum=15;

double count_front_left=0;
double count_front_right=15;
double count_rear_left=0;
double count_rear_right=15;
double count_axis=0;

void walk() {

  if (count_front_left>maximum) {
    bool_front_left=true;
  }
  if (count_front_left<-maximum) {
    bool_front_left=false;
  }

  if (count_front_right>maximum) {
    bool_front_right=true;
  }
  if (count_front_right<-maximum) {
    bool_front_right=false;
  }

  if (count_rear_left>maximum) {
    bool_rear_left=true;
  }
  if (count_rear_left<-maximum) {
    bool_rear_left=false;
  }

  if (count_rear_right>maximum) {
    bool_rear_right=true;
  }
  if (count_rear_right<-maximum) {
    bool_rear_right=false;
  }

  if (count_axis>maximum) {
    bool_axis=true;
  }
  if (count_axis<-maximum) {
    bool_axis=false;
  }
  

  if (bool_front_left) {
    count_front_left--;
  } else {
    count_front_left++;
  }
  if (bool_front_right) {
    count_front_right--;
  } else {
    count_front_right++;
  }
  if (bool_rear_left) {
    count_rear_left--;
  } else {
    count_rear_left++;
  }
  if (bool_rear_right) {
    count_rear_right--;
  } else {
    count_rear_right++;
  }

  if (bool_axis) {
    count_axis--;
  } else {
    count_axis++;
  }

  frontLeft(count_front_left+15);
  frontRight(count_front_right+15);
  rearLeft(count_rear_left-15);
  rearRight(count_rear_right-15);

  axis(count_axis);

  delay(20);

}

void sitStandStay() {
  sit();
  delay(2000);
  stay();
  delay(2000);
  stand();
  delay(2000);
  stay();
  delay(2000);
}

void sit() {
  axis(0);
  frontLeft(60);
  frontRight(60);
  rearLeft(-70);
  rearRight(-70);
}

void loop() {
  walk();

  //stand();

  //servo5.write(angle+correction_axis);
  

  //sitStandStay();
  //maizein();
  //maizein();
  //maizein();
  //dancing();
  //delay(2000);

} 