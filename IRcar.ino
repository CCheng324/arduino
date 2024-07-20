#include <Servo.h>


Servo servoL;
Servo servoR;

// 定義腳位
#define IR1 2
#define IR2 3
#define IR3 4
#define IR4 7

// 定義馬達
#define motorL 9
#define motorR 10

// 初始化函式
void setup() {
  servoL.attach(motorL);
  servoR.attach(motorR);
  
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);


}

//[Return 1 when WHITE  |  Return 0 when BLACK]

void loop() {
  int sen1 = digitalRead(IR1);
  int sen2 = digitalRead(IR2);//L

  int sen3 = digitalRead(IR3);
  int sen4 = digitalRead(IR4);//R
 
  
  if(sen2 == 0 || sen3 == 0){ // 直行
    servoL.writeMicroseconds(1800);
    servoR.writeMicroseconds(1800);
  }
  else if(sen3 == 1 && sen2 == 0){ //車子偏左
    servoL.writeMicroseconds(1850);
    servoR.writeMicroseconds(1750);
  }
  else if(sen3 == 0 && sen2 == 1){ //車子偏右
    servoL.writeMicroseconds(1750);
    servoR.writeMicroseconds(1850);
  }

}





