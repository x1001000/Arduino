#include <math.h>
#include <Servo.h>
Servo servoNumber[4];

float L = 8.1, R = 8.1;
float thetaL, thetaR;
float P, H, D;

int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle, inputPin
  { 10, 180, 90, 180, 0},                               //左
  { 11, map(90,  0, 180,  2, 159),   0, 180, 1},        //下
  {  9, map(90, 45, 180, 45, 150),  45, 180, 2},        //右
  {  8, 157, 144, 170, 3},                              //前
};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    servoNumber[i].attach(servoConfig[i][0]);
    servoNumber[i].write(servoConfig[i][1]);
  }
}

void loop() {
  while (Serial.available() == 0) {}
  P =  Serial.parseFloat();
  Serial.println(P);
  while (Serial.available() == 0) {}
  H = Serial.parseFloat();
  Serial.println(H);
  while (Serial.available() == 0) {}
  D = Serial.parseFloat();
  Serial.println(D);

  H -= 9.5;
  float f1, f2 = 0;
  for (int theta = 900; theta <= 1800; theta++) {
    thetaL = (float)theta / 10 * PI / 180;
    thetaR = thetaL + 2 * atan(0 - H / D);
    f1 = 8 * cos(thetaR) + 8 * cos(thetaL) + D;
    if (f1 * f2 < 0) {
      Serial.println(f2, 6);
      Serial.println(f1, 6);
      break;
    }
    f2 = f1;
  }
  thetaL = thetaL * 180 / PI;
  thetaR = thetaR * 180 / PI;
  thetaR = map(thetaR, 45, 180, 45, 150);
  servoNumber[0].write(thetaL);
  servoNumber[2].write(thetaR);
  P = map(P, 0, 180,  2, 159);
  servoNumber[1].write(P);
}
