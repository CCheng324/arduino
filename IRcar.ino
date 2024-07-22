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
#define det 7
#define fro 11
// 定義馬達
#define motorL 9
#define motorR 10
// 馬達轉速
int basic_motor_L = 1700;
int basic_motor_R = 1782;
int turn = 1750;
int stop_speed = 1500;
int V-shape_ turn= 1800;
int oblique_L = 1600;
int oblique_R = 1682;

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
  pinMode(det, INPUT);
  pinMode(fro, INPUT);

}

//white=0 black=1
// R to L [1 2 3 4 5]
void loop() {
  int sen1 = digitalRead(S1);
  int sen2 = digitalRead(S2);
  int sen3 = digitalRead(S3);
  int sen4 = digitalRead(S4);
  int sen5 = digitalRead(S5);
  int detect = digitalRead(det);
  int front = digitalRead(fro);

  int16_t ax, ay, az;
  
  mpu.getAcceleration(&ax, &ay, &az);
  float oblique = atan2(ay, az) * 180 / PI;
}
  
  if(sen2 == 1 && sen4 == 1 ) { // 直行
    if(oblique >= 10){  //判斷傾斜
    servoL.writeMicroseconds(oblique_L);
    servoR.writeMicroseconds(oblique_R);  
  }
  else{
    servoL.writeMicroseconds(basic_motor_L);
    servoR.writeMicroseconds(basic_motor_R);    
    }
  }
  else if((sen3 == 0 && sen1 == 0) || (sen3 == 0 && sen2 == 0)){//V-shape(右彎)
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(V-shape_ turn);      
  }
  else if((sen3 == 0 && sen5 == 0) || (sen3 == 0 && sen4 == 0)){//V-shape(左彎)
    servoL.writeMicroseconds(V-shape_ turn);
    servoR.writeMicroseconds(stop_speed);      
  }  


  /*
  else if(sen1 == 0 && sen2 == 1 ) { // 車子偏右 先延遲0.2s 再判斷  時間要實際試過再改
    delay(200);
    if(//如果 中間有感測到) {
      servoL.writeMicroseconds(stop_speed);
      servoR.writeMicroseconds(turn);
    }
    else{
      servoL.writeMicroseconds(basic_motor_L);
      servoR.writeMicroseconds(basic_motor_R);  
    }
  }
  else if(sen1 == 1 && sen2 == 0) { // 車子偏左  先延遲0.2s 再判斷  時間要實際試過再改
    delay(200);
    if(//如果 中間有感測到){
      servoL.writeMicroseconds(turn);
      servoR.writeMicroseconds(stop_speed);
    }
    else{
      servoL.writeMicroseconds(basic_motor_L);
      servoR.writeMicroseconds(basic_motor_R); 
    }  
  */


  }
  else if(sen1 == 0 && sen2 == 0){
    if (detect == 1){
      servoL.writeMicroseconds(basic_motor_L);
      servoR.writeMicroseconds(basic_motor_R);      
  }
   else{
    servoL.writeMicroseconds(stop_speed);
    servoR.writeMicroseconds(stop_speed);    
    }
    }  





