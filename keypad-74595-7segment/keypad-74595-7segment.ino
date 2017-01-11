//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __|
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/
//
// keypad-74595-7segment
//
// Made by 系統師 育成高中
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3660379-keypad-74595-7segment

#include <Keypad.h>

byte rowPins[] = { 9, 8, 7, 6 };
byte colPins[] = { 5, 4, 3, 2 };

char keys[][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

byte clockPin = 11; // output to SH_CP of 74HC595
byte latchPin = 12; // output to ST_CP of 74HC595
byte dataPin = 13;  // output to DS    of 74HC595

byte code[] = { // Common Cathode 7-segment
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B10111110,
  B11100000,
  B11111110,
  B11110110
};

char key, sevenSegment[3];
int doomTime;

void getAllzeros() {
  sevenSegment[0] = '0';
  sevenSegment[1] = '0';
  sevenSegment[2] = '0';
  updateSevenSegment();
}

void getBack() {
  sevenSegment[2] = sevenSegment[1];
  sevenSegment[1] = sevenSegment[0];
  sevenSegment[0] = 0;  // code[ 0 - '0' ] == 0
  updateSevenSegment();
}

void getClear() {
  sevenSegment[0] = 0;  // code[ 0 - '0' ] == 0
  sevenSegment[1] = 0;  // code[ 0 - '0' ] == 0
  sevenSegment[2] = 0;  // code[ 0 - '0' ] == 0
  updateSevenSegment();
}

void getDoom() {
  doomTime = 1;
  if (sevenSegment[0] - '0' >= 0) doomTime += (sevenSegment[0] - '0') * 100;
  if (sevenSegment[1] - '0' >= 0) doomTime += (sevenSegment[1] - '0') * 10;
  if (sevenSegment[2] - '0' >= 0) doomTime += (sevenSegment[2] - '0') * 1;
  while (doomTime--) {  // use loops when there are more than 3 7-segments
    sevenSegment[0] = '0' + (doomTime % 1000) / 100;
    sevenSegment[1] = '0' + (doomTime % 100) / 10;
    sevenSegment[2] = '0' + (doomTime % 10) / 1;
    updateSevenSegment();
    delay(1000);
  }
}

void updateSevenSegment() {
  digitalWrite(latchPin, LOW);
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode 7-segment
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode 7-segment
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode 7-segment
  digitalWrite(latchPin, HIGH);
  delay(100);     // to avoid the Persistence of vision
  digitalWrite(latchPin, LOW);
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[0] - '0'] ); // Common Anode 7-segment
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[1] - '0'] ); // Common Anode 7-segment
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[2] - '0'] ); // Common Anode 7-segment
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode( clockPin, OUTPUT );
  pinMode( latchPin, OUTPUT );
  pinMode( dataPin, OUTPUT );
  getClear();
  Serial.begin(9600);
}

void loop() {
  if (key = kpd.getKey()) {
    Serial.println(key);
    if (key == 'A')
      getAllzeros();
    else if (key == 'B')
      getBack();
    else if (key == 'C')
      getClear();
    else if (key == 'D')
      getDoom();
    else {
      sevenSegment[0] = sevenSegment[1];
      sevenSegment[1] = sevenSegment[2];
      sevenSegment[2] = key;
      updateSevenSegment();
    }
  }
}
