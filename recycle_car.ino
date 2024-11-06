#include <Servo.h>
#include <Ultrasonic.h>

Servo fan;
Servo motL;
Servo motR;

int fanPin = 4; 
int IR0 = A0;
int IR1 = A1;
int IR2 = A2;
int IR3 = A3;
Ultrasonic sr1(8, 7); 
Ultrasonic sr2(10, 9); 
Ultrasonic sr3(12, 11); 

int motLPin = 5;
int motRPin = 6;

int distance1, distance2, distance3;
int fanThreshold = 20;

void setup() {
  Serial.begin(9600);
  
  fan.attach(fanPin); 
  motL.attach(motLPin);
  motR.attach(motRPin);
  
  fan.write(0); 
  motL.write(90); 
  motR.write(90); 
  
  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  
  delay(2000); 
}

void loop() {
  // 讀取超音波感測器的距離
  distance1 = sr1.read(); 
  distance2 = sr2.read(); 
  distance3 = sr3.read(); 

  // 讀取並反轉 IR 傳感器的值
  int irValue0 = !digitalRead(IR0);
  int irValue1 = !digitalRead(IR1);
  int irValue2 = !digitalRead(IR2);
  int irValue3 = !digitalRead(IR3);

  // 檢查是否任何一個 IR 傳感器被觸發
  if (irValue0 || irValue1 || irValue2 || irValue3) {
    if (irValue0 || irValue1) {
      reverse();
      delay(500);
      turnRight();
      delay(500);
    } else if (irValue2 || irValue3) {
      moveForward();
      delay(500);       
    }
  } else {
    // IR 無觸發，使用超音波距離
    if (distance2 < fanThreshold) {
      activateFan();
      moveForward();
      Serial.println("rush");
    } else if (distance1 < fanThreshold) {
      deactivateFan();
      turnLeft();
      Serial.println("left");
    } else if (distance3 < fanThreshold) {
      deactivateFan();
      turnRight();
      Serial.println("right");
    } else {
      moveForward();
      Serial.println("else");
    }
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm, Distance 3: ");
  Serial.println(distance3);
 
  }

  delay(200); 
}


void reverse() {
  motL.write(20);    
  motR.write(20);  
}

void moveForward() {
  motL.write(160);
  motR.write(160);
}

void turnLeft() {
  motL.write(90); 
  motR.write(160); 
}

void turnRight() {
  motL.write(160); 
  motR.write(90); 
}

void activateFan() {
  fan.write(180); 
}

void deactivateFan() {
  fan.write(0); 
}
