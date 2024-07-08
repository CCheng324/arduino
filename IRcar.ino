#include <Servo.h>

//BDF-1000
#define S1 D2
#define S2 D3
#define S3 D4
#define S4 D7
#define S5 D8
//HW201*4
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4
//motor
#define motorL D9
#define motorR D10

Servo.servoL;
Servo.servoR;


void setup() {
  servoL.attach(motorL);
  servoR.attach(motorR);
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);
  pinMode(S5,INPUT);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(IR4,INPUT);
  pinMode(motorL,OUTPUT);
  pinMode(motorR,OUTPUT);
  
}

//循跡線是白線 白色：紅外線感測器返回1 黑色：返回0
//LtoR S1 to S5 S3在最中間 先寫五路感測器

void loop() {
  sensors1=digitalRead(S1);
  sensors2=digitalRead(S2);
  sensors3=digitalRead(S3);
  sensors4=digitalRead(S4);
  sensors5=digitalRead(S5);
  sensorir1=digitalRead(IR1);
  sensorir2=digitalRead(IR2);
  sensorir3=digitalRead(IR3);
  sensorir4=digitalRead(IR4);
  
  if(sensors3==1 && sensors1==0 && sensors2==0 && sensors4 ==0 && sensors5==0){
forward();
}

}

//1000後 1500停 2000前

void forward(){
  servoL.writeMicroseconds(2000);
  servoR.writeMicroseconds(2000);
}

void stop(){
  servoL.writeMicroseconds(1500);
  servoR.writeMicroseconds(1500);
}

void back(){
  servoL.writeMicroseconds(1000);
  servoR.writeMicroseconds(1000);
}

void right(){
  servoL.writeMicroseconds(1800);
  servoR.writeMicroseconds(1500);
}

void left(){
  servoL.writeMicroseconds(1500);
  servoR.writeMicroseconds(1800);
}

