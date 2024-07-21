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
#define S5 8
// 定義馬達
#define motorL 9
#define motorR 10

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

void loop() {
  int sen1 = digitalRead(S1);
  int sen2 = digitalRead(S2);
  int sen3 = digitalRead(S3);
  int sen4 = digitalRead(S4);
  int16_t ax, ay, az;
  
  mpu.getAcceleration(&ax, &ay, &az);
  float oblique = atan2(ay, az) * 180 / PI;
  
  Serial.print("Roll = "); 
  Serial.println(roll);
  Serial.print("S4: ");
  Serial.println(sen4);

  delay(1000);
}
  
  if(sen1 == 1 && sen2 == 1 ) { // 直行
    if(oblique >= 10){
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1582);       
  }
  else{
    servoL.writeMicroseconds(1600);
    servoR.writeMicroseconds(1682);    
    }

  }
  else if(sen1 == 0 && sen2 == 1 ) { // 車子偏左
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1750);
  }
  else if(sen1 == 1 && sen2 == 0) { // 車子偏右
    servoL.writeMicroseconds(1750);
    servoR.writeMicroseconds(1500);
  }
  else if(sen1 == 0 && sen2 == 0){
    if (sen4 == 1){
    servoL.writeMicroseconds(1600);
    servoR.writeMicroseconds(1682);      
  }
   else{
    servoL.writeMicroseconds(1500);
    servoR.writeMicroseconds(1500);    
    }
    }  





