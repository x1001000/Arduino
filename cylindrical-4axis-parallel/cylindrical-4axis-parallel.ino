#include <math.h>
#include <Servo.h>
Servo servoNumber[4];

const float P0 = 90, H0 = 1, D0 = 12;
const float L = 8.1, R = 8.1;
const int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle, inputPin
  { 10, 180, 90, 180, 0},                               //左
  {  9, map(90, 45, 180, 45, 150),  45, 180, 2},        //右
  {  8, 157, 144, 170, 3},                              //前
  {  3, map(90,  0, 180,  2, 156),   0, 180, 1},        //下
};

void servo(float P, float H, float D) {
  float thetaL, thetaR;
  float f1, f2 = 0;
  for (int theta = 900; theta <= 1800; theta++) {
    thetaL = (float)theta / 10 * PI / 180;
    thetaR = thetaL + 2 * atan(0 - H / D);
    f1 = L * cos(thetaL) + R * cos(thetaR) + D;
    if (f1 * f2 < 0) {
      //Serial.println(f2, 6);
      //Serial.println(f1, 6);
      break;
    }
    f2 = f1;
  }
  thetaL = thetaL * 180 / PI;
  thetaR = thetaR * 180 / PI;
  thetaR = map(thetaR, 45, 180, 45, 150);
  servoNumber[0].write(thetaL);
  servoNumber[1].write(thetaR);
  servoNumber[3].write(map(P,  0, 180,  2, 156));
  delay(50);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    servoNumber[i].attach(servoConfig[i][0]);
    servoNumber[i].write(servoConfig[i][1]);
  }
  delay(1000);
}

void loop() {
  for (int P = 120; P >= 60; P -= 4) servo(P, H0, D0 / cos((P - 90) * PI / 180));
  setup();
  for (float D = D0; D >= 4; D -= 0.4) servo(P0, H0, D);
  setup();
}
