#include <Servo.h>
#include <PID_v1.h>

Servo servoL;
Servo servoR;

// 红外传感器引脚定义
#define S1 D2
#define S2 D3
#define S3 D4
#define S4 D7
#define S5 D8
#define IR1 A1
#define IR2 A2
#define IR3 A3
#define IR4 A4

// 电机引脚定义
#define motorL D9
#define motorR D10

// PID控制变量
double Setpoint = 0; // 设定点，理想情况下偏差为0
double Input, Output;
double Kp = 1.0, Ki = 0.1, Kd = 0.0; // PID参数
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// 捷径检测变量
bool shortcutDetected = false;

// 初始化函数
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
  
  myPID.SetMode(AUTOMATIC); // 设置PID模式为自动调节
}

// 主循环
void loop() {
  // 读取传感器值
  int sen1 = digitalRead(S1);
  int sen2 = digitalRead(S2);
  int sen3 = digitalRead(S3);
  int sen4 = digitalRead(S4);
  int sen5 = digitalRead(S5);
  
  // 计算加权差值
  int error = calculateError(sen1, sen2, sen3, sen4, sen5);

  // 检测捷径
  shortcutDetected = detectShortcut();

  // 如果检测到捷径，调整PID参数
  if (shortcutDetected) {
    // 根据捷径情况调整 Setpoint 和 PID 控制
    adjustForShortcut();
  } else {
    // 否则，使用正常的PID控制
    Input = error; // 设置PID输入
    myPID.Compute(); // 计算PID输出
    controlMotors(Output); // 控制电机
  }

  // 延时
  delay(50);
}

// 计算偏差函数
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

// 检测捷径函数
bool detectShortcut() {
  int threshold = 500; // 根据实际情况调整阈值
  
  // 读取捷径传感器值
  int ir1 = analogRead(IR1);
  int ir2 = analogRead(IR2);
  int ir3 = analogRead(IR3);
  int ir4 = analogRead(IR4);
  
  // 判断是否检测到捷径
  bool leftShortcut = (ir1 < threshold) || (ir2 < threshold);
  bool rightShortcut = (ir3 < threshold) || (ir4 < threshold);
  
  return leftShortcut || rightShortcut;
}

// 根据捷径调整函数
void adjustForShortcut() {
  int baseSpeed = 150; // 基础速度，根据需要调整
  
  // 根据捷径传感器调整方向
  int leftMotorSpeed = baseSpeed;
  int rightMotorSpeed = baseSpeed;
  
  if (analogRead(IR1) < 500 || analogRead(IR2) < 500) {
    // 左侧捷径，向左转
    servoL.writeMicroseconds(1000); // 向左
    servoR.writeMicroseconds(2000); // 向前
  } else if (analogRead(IR3) < 500 || analogRead(IR4) < 500) {
    // 右侧捷径，向右转
    servoL.writeMicroseconds(2000); // 向前
    servoR.writeMicroseconds(1000); // 向右
  }
  
  // 设置PID输入为0，保持原地
  Input = 0;
}

// 控制电机函数
void controlMotors(double pidOutput) {
  int baseSpeed = 150; // 基础速度，根据需要调整
  int leftMotorSpeed = constrain(baseSpeed - pidOutput, 0, 255);
  int rightMotorSpeed = constrain(baseSpeed + pidOutput, 0, 255);
  
  // 控制左电机
  if (pidOutput >= 0) {
    servoL.writeMicroseconds(2000); // 向前
  } else {
    servoL.writeMicroseconds(1000); // 向后
  }
  
  // 控制右电机
  if (pidOutput >= 0) {
    servoR.writeMicroseconds(2000); // 向前
  } else {
    servoR.writeMicroseconds(1000); // 向后
  }
}
