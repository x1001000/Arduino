//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __|
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/
//
// keypad-74595-7segment
//
// Made by 系統師 育成高中
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/3660379-arduino-keypad

#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const byte clockPin = 11;  //Pin connected to SH_CP of 74HC595
const byte latchPin = 12;  //Pin connected to ST_CP of 74HC595
const byte dataPin = 13;   //Pin connected to DS    of 74HC595

const byte code[] = {	// for Common Cathode 7-segment
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

void display() {
  digitalWrite(latchPin, LOW);
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 );	// Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode
  digitalWrite(latchPin, HIGH);
  delay(100);								// clear
  digitalWrite(latchPin, LOW);
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[2] - '0'] ); // to Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[1] - '0'] ); // to Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 - code[sevenSegment[0] - '0'] ); // to Common Anode
  digitalWrite(latchPin, HIGH);
}

void setup() {
  pinMode( clockPin, OUTPUT );
  pinMode( latchPin, OUTPUT );
  pinMode( dataPin, OUTPUT );
  digitalWrite(latchPin, LOW);
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 );	// Common Anode
  shiftOut( dataPin, clockPin, LSBFIRST, 255 ); // Common Anode
  digitalWrite(latchPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  if (key = kpd.getKey()) {
    Serial.println(key);
    if ( key - '0' >= 0 && key - '0' <= 9 ) {
      sevenSegment[2] = sevenSegment[1];
      sevenSegment[1] = sevenSegment[0];
      sevenSegment[0] = key;
      display();
    }
  }
}
