#define UP -10000
#define DOWN 10000
#define RIGHT -10000
#define LEFT 10000
#define PX 2
#define PY 0

int x, y;
int x_last, y_last;

int matrix[4][3]={  {-1, 5,-1},
                    { 1, 4, 7},
                    { 0, 3, 6},
                    {-1, 2,-1}};
int px=PX, py=PY;       
int pos;
int JoyStick_X = A1;
int JoyStick_Y = A0;
const byte NumPin = 2;
const byte RtPin = 3;
const byte RsPin = 4;
const byte R_pin = 9;
const byte G_pin = 10;
const byte B_pin = 11;
byte R = 0, G = 0, B = 0;

int num = 0;
byte clic = 0;
int rgb[3]={255,0,0};
const byte sw = 7;

boolean z;
boolean z_last = LOW;

const int RGB_sel[] =
{
  B01111111, B10111111, B11011111, B11101111,
  B11110111, B11111011, B11111101, B11111110
};
void Led_show(byte R, byte G, byte B, int pos)
{
  digitalWrite(RtPin, LOW);
  shiftOut(NumPin, RsPin, LSBFIRST, RGB_sel[pos]);
  digitalWrite(RtPin, HIGH);

  analogWrite(R_pin, R);
  analogWrite(G_pin, G);
  analogWrite(B_pin, B);
  delay(2);
}

bool isUp(){return (x_last > UP/2 && x < UP/2)?true:false;}
bool isDown(){return (x_last < DOWN/2 && x > DOWN/2)?true:false;}
bool isLeft(){return (y_last < LEFT/2 && y > LEFT/2)?true:false;}
bool isRight(){return (y_last > RIGHT/2 && y < RIGHT/2)?true:false;}
bool isPush(){return (z_last==false && z==true)?true:false;}

void moveUp(){px-=1; px+=4; px%=4; if(matrix[px][py]==-1) moveUp();}
void moveDown(){px+=1;px+=4;px%=4; if(matrix[px][py]==-1) moveDown();}
void moveLeft(){py-=1;py+=3;py%=3; if(matrix[px][py]==-1) moveLeft();}
void moveRight(){py+=1;py+=3;py%=3; if(matrix[px][py]==-1) moveRight();}
void changeColor(){clic+=1;}

void setup()
{
  Serial.begin(9600);
  for (byte i = 2; i < 5; i++)
  {
    pinMode(i, OUTPUT);
  }
  for (byte i = 9; i < 12; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);
  Serial.begin(9600);

  pinMode(sw, INPUT);
  for (byte i = 7; i < 14; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}
void loop()
{
  x = map(analogRead(JoyStick_X), 0, 1023, UP, DOWN);
  y = map(analogRead(JoyStick_Y), 0, 1023, RIGHT, LEFT);
  z = digitalRead(sw);
/*
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(",");
 */
  pos = matrix[px][py];
  Led_show(rgb[clic%3], rgb[(clic+1)%3], rgb[(clic+2)%3], pos);
  if(isUp()) moveUp();
  if(isDown()) moveDown();
  if(isLeft()) moveLeft();
  if(isRight()) moveRight();
  if(isPush()) changeColor();
  x_last = x;
  y_last = y;
  z_last = z;
}
