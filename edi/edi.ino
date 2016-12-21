//https://circuits.io/circuits/3566596-rewrite-edi-s-project
byte onLED = 0;

void moveLED(int t, int n)
{
  while (n--)                     //重複n次
  {
    digitalWrite(5 + onLED, LOW); //關
    onLED++;
    onLED %= 8;
    digitalWrite(5 + onLED, HIGH);//開t毫秒
    delay(t);
  }
}

void setup()
{
  pinMode(2, INPUT);            //設定pin2為INPUT (from 按鈕)
  for (int i = 5; i < 13; i++)  //設定pin5~12為OUTPUT (to 8個燈)
    pinMode(i, OUTPUT);
  digitalWrite(5, HIGH);        //先亮第一顆燈
  randomSeed(analogRead(0));    //改變亂數種子
}

void loop()
{
  while (digitalRead(2) == LOW) delay(1);   //等你按
  while (digitalRead(2) == HIGH) delay(20); //等你放
  moveLED(250, 8);                //啟動跑1圈
  moveLED(100, 8 * random(2, 4)); //快速跑2~3圈
  moveLED(200, 8);                //慢速跑1圈
  moveLED(350, 8 + random(0, 8)); //結束跑1圈多0~7顆
}
