#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

Servo servoL;
Servo servoR;
MPU6050 mpu;

// 定義腳位
#define S1 2
#define S2 3
#define S3 4
#define S4 5
#define S5 6 
// 定義馬達
#define motorL 9
#define motorR 10
// 馬達轉速
int basic_motor_L = 1700;
int basic_motor_R = 1782;
int turn = 1750;
int stop_speed = 1500;
int Vshape_turn = 1800;

// 初始化函式
void setup() {
  Serial.begin(9600);
  servoL.attach(motorL);
  servoR.attach(motorR);
  Wire.begin();
  mpu.initialize();
  
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
}

// 獲取感測器數據
void getSensorData(int &sen1, int &sen2, int &sen3, int &sen4, int &sen5) {
  sen1 = digitalRead(S1);
  sen2 = digitalRead(S2);
  sen3 = digitalRead(S3);
  sen4 = digitalRead(S4);
  sen5 = digitalRead(S5);
}

// 獲取傾斜角度
float getObliqueAngle() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  return atan2(ay, az) * 180 / PI;
}

// 根據傾斜角度調整馬達轉速
void detectOblique(float obl) {
  if (obl > 10) {
    basic_motor_L = 1600;
    basic_motor_R = 1682;
  } else if (obl < -20) {
    basic_motor_L = 1800;
    basic_motor_R = 1882;
  } else {
    basic_motor_L = 1700;
    basic_motor_R = 1782;
  }
}

// 控制車子轉向
void controlCar(int sen1, int sen2, int sen3, int sen4, int sen5) {
  if (sen1 == 1 && sen2 == 1 && sen3 == 1 && sen4 == 1 && sen5 == 1) { // 全黑停車
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(stop_speed);
  } else if (sen1 == 1 && sen2 == 1 && sen3 == 1 && sen4 == 1) {
    servoL.writeMicroseconds(Vshape_turn);
    servoR.writeMicroseconds(stop_speed);
  } else if (sen2 == 1 && sen3 == 1 && sen4 == 1 && sen5 == 1 ) { 
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(Vshape_turn);    






    
  } else if (sen3 == 1 && (sen1 == 1 || sen2 == 1)) {// T(右彎)
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(stop_speed);
    delay(100);
    servoL.writeMicroseconds(Vshape_turn);
    servoR.writeMicroseconds(stop_speed);
    delay(500);
  } else if (sen3 == 1 && (sen4 == 1 || sen5 == 1)) { //T(左彎)
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(stop_speed);
    delay(100);    
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(Vshape_turn);
    delay(500);
  } else if ((sen2 == 0 && sen4 == 1) || sen5 == 1) { // 車子偏右 
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(turn);
  } else if ((sen2 == 1 && sen4 == 0) || sen1 == 1) { // 車子偏左 
    servoL.writeMicroseconds(turn);
    servoR.writeMicroseconds(stop_speed);
  } else if (sen1 == 1) { // 車子偏左 
    servoL.writeMicroseconds(turn);
    servoR.writeMicroseconds(stop_speed);
  } else if (sen5 == 1) { // 車子偏右 
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(turn);
  } else { // 直行
    servoL.writeMicroseconds(basic_motor_L);
    servoR.writeMicroseconds(basic_motor_R);
  }
}

void loop() {
  int sen1, sen2, sen3, sen4, sen5;
  getSensorData(sen1, sen2, sen3, sen4, sen5);

  float oblique = getObliqueAngle();
  detectOblique(oblique);

  controlCar(sen1, sen2, sen3, sen4, sen5);
}
