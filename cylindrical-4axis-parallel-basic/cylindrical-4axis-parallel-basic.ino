#include <Servo.h>
Servo servo[4];
int inputSignal[4];
int outputAngle[4];
const int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle, inputPin
  { 11, 135,  90, 180, 0}, //左
  { 10, 115,  50, 180, 2}, //右
  {  9, 150, 135, 165, 3}, //前
  {  4,  85,   0, 180, 1}, //下  http://playground.arduino.cc/Main/ArduinoPinCurrentLimitations
};
void measureRangeOfServo(int i) {
  int value = 0;
  while (Serial.available())
  {
    value *= 10;
    value += Serial.read() - '0';
  }
  if (value != 0)
    servo[i].write(value);
  delay(100);
}
void joystickInput() {
  for (int i = 0; i < 4; i++)
    inputSignal[i] = map(analogRead(servoConfig[i][4]), 0, 1023, -4, 5);
}
void servoOutput() {
  for (int i = 0; i < 4; i++) {
    if (i == 1)
      inputSignal[i] *= -1;                                         //修正右servo轉向
    if (inputSignal[i])
      if (outputAngle[i] - inputSignal[i] > servoConfig[i][2])
        if (outputAngle[i] - inputSignal[i] < servoConfig[i][3]) {  //預檢有無超出範圍
          outputAngle[i] -= inputSignal[i];                         //配合左下servo轉向
          servo[i].write(outputAngle[i]);                           //4or5段速
          if (i == 3)
            delay(700);                                             //下servo快轉會當機
          else
            delay(20);
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
