#include <Servo.h>
#include <PID_v1.h>

Servo servoL;
Servo servoR;

// 定義腳位
#define S1 D2
#define S2 D3
#define S3 D4
#define S4 D7
#define S5 D8
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4

// 定義馬達
#define motorL D9
#define motorR D10

// PID變量
double Setpoint = 0; // 設定目標值，應為0
double Input, Output;
double Kp = 1.0, Ki = 0.1, Kd = 0.0; // PID參數設置
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// 捷徑檢測變量
bool shortcutDetected = false;

// 初始化函式
void setup() {
  servoL.attach(motorL);
  servoR.attach(motorR);
  
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  
  myPID.SetMode(AUTOMATIC); // 設置PID模式為自動調節
}


void loop() {
  int sen1 = digitalRead(S1);
  int sen2 = digitalRead(S2);
  int sen3 = digitalRead(S3);
  int sen4 = digitalRead(S4);
  int sen5 = digitalRead(S5);
  
  // error值計算
  int error = calculateError(sen1, sen2, sen3, sen4, sen5);

  // 是否有捷徑
  shortcutDetected = detectShortcut();

  // 如果有捷徑調整PID
  if (shortcutDetected) {
    adjustForShortcut();
  } else {
    // 否則，使用正常的PID控制
    Input = error; // 設置PID輸入
    myPID.Compute(); // 計算PID輸出
    controlMotors(Output); // 控制馬達
  }
  delay(50);
}

// 計算error
int calculateError(int sen1, int sen2, int sen3, int sen4, int sen5) {
  if (sen1 == 1 && sen2 == 0 && sen3 == 0 && sen4 == 0 && sen5 == 0) // 10000
    return 3;
  else if ((sen1 == 1 && sen2 == 1 && sen3 == 0 && sen4 == 0 && sen5 == 0) || (sen1 == 1 && sen2 == 1 && sen3 == 0 && sen4 == 1 && sen5 == 0)) // 11000 or 11100
    return 2;
  else if (sen1 == 0 && sen2 == 1 && sen3 == 0 && sen4 == 0 && sen5 == 0) // 01000
    return 1;
  else if ((sen1 == 0 && sen2 == 0 && sen3 == 1 && sen4 == 0 && sen5 == 0) || (sen1 == 0 && sen2 == 1 && sen3 == 1 && sen4 == 1 && sen5 == 0)) // 00100 or 01110
    return 0;
  else if (sen1 == 0 && sen2 == 0 && sen3 == 0 && sen4 == 1 && sen5 == 0) // 00010
    return -1;
  else if ((sen1 == 0 && sen2 == 0 && sen3 == 0 && sen4 == 1 && sen5 == 1) || (sen1 == 0 && sen2 == 0 && sen3 == 1 && sen4 == 1 && sen5 == 1)) // 00011 or 00111
    return -2;
  else if (sen1 == 0 && sen2 == 0 && sen3 == 0 && sen4 == 0 && sen5 == 1) // 00001
    return -3;
  else
    return 0;
}

// 捷徑函式
bool detectShortcut() {
  int threshold = 500; 

  int ir1 = analogRead(IR1);
  int ir2 = analogRead(IR2);
  int ir3 = analogRead(IR3);
  int ir4 = analogRead(IR4);
  
  // 捷徑(true or false)
  bool leftShortcut = (ir1 < threshold) || (ir2 < threshold);
  bool rightShortcut = (ir3 < threshold) || (ir4 < threshold);
  
  return leftShortcut || rightShortcut;
}

// 如果遇到十字路口該麼辦?(未解決)


void adjustForShortcut() {
  // 根據捷径感測器調整方向
  if (analogRead(IR1) < 500 || analogRead(IR2) < 500) {
    // 左側捷徑，向左轉
    servoL.writeMicroseconds(1500); 
    servoR.writeMicroseconds(2000); 
  } else if (analogRead(IR3) < 500 || analogRead(IR4) < 500) {
    // 右側捷徑，向右轉
    servoL.writeMicroseconds(2000); 
    servoR.writeMicroseconds(1500); 
  }
  Input = 0;
}

// 控制馬達函式
void controlMotors(double pidOutput) {
  int baseSpeed = 1800; 

  // 根據 PID 輸出調整左右馬達的舵機脈衝寬度
  int leftMotorSpeed = constrain(baseSpeed - pidOutput, 1500, 2000); // 限制在1500到2000微秒之間
  int rightMotorSpeed = constrain(baseSpeed + pidOutput, 1500, 2000); // 限制在1500到2000微秒之間
  
  servoL.writeMicroseconds(leftMotorSpeed);
  servoR.writeMicroseconds(rightMotorSpeed);
}
