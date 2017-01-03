// 範圍限制尚未處理
#include <Servo.h>
Servo servo[4];
int inputSignal[4];
int outputAngle[4];
const int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle, inputPin
  { 11, 135,  90, 180, 0}, //左
  { 10, 125,  70, 180, 2}, //右
  {  9, 150, 135, 165, 3}, //前
  {  8, 85,    5, 155, 1}, //下
};
void measureRangeOfServo(int i) {
  int value = 0;
  while (Serial.available())
  {
    value *= 10;
    value += Serial.read() - '0';
  }
  if (value != 0) servo[i].write(value);
  delay(100);
}
void joystickInput() {
  for (int i = 0; i < 4; i++)
    inputSignal[i] = map(analogRead(servoConfig[i][4]), 0, 1023, -1, 2);
}
void servoOutput() {
  for (int i = 0; i < 4; i++) {
    if (inputSignal[i]) {
      outputAngle[i] -= inputSignal[i];   //配合下servo的轉向
      servo[i].write(outputAngle[i]);
      delay(10);
    }
  }
}
void setup() {
  for (int i = 0; i < 4; i++) {
    servo[i].attach(servoConfig[i][0]);
    servo[i].write(servoConfig[i][1]);
    outputAngle[i] = servoConfig[i][1];
  }
  Serial.begin(9600);
}
void loop() {
  //while (1) measureRangeOfServo(2);
  joystickInput();
  servoOutput();
}
