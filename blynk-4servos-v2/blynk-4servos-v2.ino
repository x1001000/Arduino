#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>
char auth[] = "76fc0283442d46f697722886d013bd25";

#include <Servo.h>
Servo servoNumber[4];
int servoConfig[4][5] = {
  //outputPin, initAngle, minAngle, maxAngle
  { 11,        80,         0,        160},    //11=下
  { 10,       140,       100,        180},    //10=左
  {  9,       100,        50,        150},    // 9=右
  {  8,       144,       144,        170},    // 8=前
};
int input[6], output[4];                      // 6 input signals, 4 output signals

void setup() {
  SwSerial.begin(9600);
  Blynk.begin(auth);
  for (int i = 0; i < 4; i++) {
    servoNumber[i].attach(servoConfig[i][0]),1000,2000);  // Std servo
    servoNumber[i].write(servoConfig[i][1]);
    output[i] = servoConfig[i][1];
  }
}

int bottomAngle(){
  return 
}

void loop() {
  //Blynk.run();
  for (int i = 0; i < 6; i++) {                 // input 6 signals
    if (i < 4)  input[i] = analogRead(i);
    else  input[i] = digitalRead(i);
  }
  if(input[4])  servoNumber[0].write(bottomAngle());
  if(input[5]&&output[3]>servoConfig[3][2])  servoNumber[3].write(output[3]--);
  else servoNumber[3].write(output[3]--);


  for (int i = 0; i < 2; i++) {
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
BLYNK_WRITE(V2) {
  input[4] = param.asInt();
}
BLYNK_WRITE(V3) {
  input[5] = param.asInt();
}
