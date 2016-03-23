#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>
char auth[] = "b45d3b06c7b2464f91ff70c85894bedd";

#include <Servo.h>
Servo servoNumber[4];
int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle, inputPin
  { 10, 140, 100, 180, 0},  //A0=LY 控 10=左
  { 11,  80,   0, 160, 1},  //A1=LX 控 11=下
  {  9, 100,  50, 150, 2},  //A2=RY 控  9=右
  {  8, 157, 144, 170, 3},  //A3=RX 控  8=前
};
int input[4], output[4];

void setup() {
  SwSerial.begin(9600);
  Blynk.begin(auth);
  for (int i = 0; i < 4; i++) {
    servoNumber[i].attach(servoConfig[i][0]);
    servoNumber[i].write(servoConfig[i][1]);
    output[i] = servoConfig[i][1];
  }
}

void loop() {
  //Blynk.run();
  for (int i = 0; i < 4; i++) {
    input[i] = analogRead(servoConfig[i][4]);
  }
  for (int i = 0; i < 4; i++) {
    if (input[i] < 312 && output[i] < servoConfig[i][3]) servoNumber[i].write(output[i]++);
    if (input[i] > 712 && output[i] > servoConfig[i][2]) servoNumber[i].write(output[i]--);
  }
  delay(10);
}

BLYNK_WRITE(V0) {
  input[0] = param[0].asInt();
  input[1] = param[1].asInt();
}
BLYNK_WRITE(V1) {
  input[2] = param[0].asInt();
  input[3] = param[1].asInt();
}
