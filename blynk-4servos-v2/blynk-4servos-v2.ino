#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>
char auth[] = "76fc0283442d46f697722886d013bd25";
#include <math.h>
#include <Servo.h>
Servo servoNumber[4];
int servoConfig[4][4] = {
  //outputPin, initAngle, minAngle, maxAngle
  { 11,        80,         0,        160},    //11=下 右 左
  { 10,       140,       100,        180},    //10=左 低 高
  {  9,       100,        50,        150},    // 9=右 近 遠
  {  8,       157,       144,        170},    // 8=前 開 闔
};
int input[6], output[4];                      // 6 input signals, 4 output signals
int offset[4], degree;
int input4 = HIGH, input5 = HIGH, mode = LOW;

void setup() {
  Serial.begin(9600);
  //SwSerial.begin(9600);
  Blynk.begin(auth);
  for (int i = 0; i < 4; i++) {
    servoNumber[i].attach((servoConfig[i][0]));
    servoNumber[i].write(servoConfig[i][1]);
    output[i] = servoConfig[i][1];
  }
  for (int i = 0; i < 4; i++)
    offset[i] = analogRead(i);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

int isRotateMode() {
  if (input4 == LOW && input[4] == HIGH) mode++, mode %= 2;
  if (input5 == LOW && input[5] == HIGH) mode++, mode %= 2;
  input4 = input[4];
  input5 = input[5];
  return mode;
}

void rotate() {
  for (int i = 0; i < 2; i++)
    input[i] -= offset[i];
  for (int i = 0; i < 2; i++) {
    if (abs(input[i]) > 480) break;
    if (i == 1) return;
  }
  degree = atan2(input[0], input[1]) * 180 / 3.14;
  if (degree > 0) servoNumber[0].write(map(degree, 1, 180, 0, 160));
}
void left() {
  if (input[0] > 923 && output[1] > servoConfig[1][2]) servoNumber[1].write(output[1]--);
  if (input[0] < 100 && output[1] < servoConfig[1][3]) servoNumber[1].write(output[1]++);
}
void right() {
  if (input[2] < 100 && output[2] > servoConfig[2][2]) servoNumber[2].write(output[2]--);
  if (input[2] > 923 && output[2] < servoConfig[2][3]) servoNumber[2].write(output[2]++);
}
void clamp() {
  if (input[3] > 923 && input[1] < 100 && output[3] > servoConfig[3][2]) servoNumber[3].write(output[3]--);
  if (input[1] > 923 && input[3] < 100 && output[3] < servoConfig[3][3]) servoNumber[3].write(output[3]++);
}

void loop() {
  //Blynk.run();
  //*input 6 signals
  for (int i = 0; i < 6; i++) {
    if (i < 4)  input[i] = analogRead(i);
    else        input[i] = digitalRead(i);
  }//*/

  if (isRotateMode()) rotate();
  else left(), right(), clamp();
  delay(10);
  //*
  Serial.print(input[0]);
  Serial.print("\t");
  Serial.print(input[1]);
  Serial.print("\t");
  Serial.print(degree);
  Serial.print("\t");
  Serial.println(isRotateMode());//*/
}

BLYNK_WRITE(V0) {
  input[0] = param[0].asInt();
  input[1] = param[1].asInt();
}
BLYNK_WRITE(V1) {
  input[2] = param[0].asInt();
  input[3] = param[1].asInt();
}
BLYNK_WRITE(V2) {
  input[4] = param.asInt();
}
BLYNK_WRITE(V3) {
  input[5] = param.asInt();
}
