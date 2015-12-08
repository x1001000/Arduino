#include "pitches.h"
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

int latchPin = 3;   //Pin connected to ST_CP of 74HC595
int clockPin = 4;  //Pin connected to SH_CP of 74HC595
int dataPin = 2;   //Pin connected to DS of 74HC595
int buttonPin = 7;
int ledGnd[4] = {12, 11 , 10, 9}; // Pin connected to led grounds
int ledInt[4];
byte seven[] = {B11111100,
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
void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledGnd[0], OUTPUT);
  pinMode(ledGnd[1], OUTPUT);
  pinMode(ledGnd[2], OUTPUT);
  pinMode(ledGnd[3], OUTPUT);
  pinMode(13, OUTPUT);
}

void scan() {
  for (int i = 0; i < 4; i++) {
    // take the latchPin low so the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, LSBFIRST, seven[ledInt[i]]);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    digitalWrite(ledGnd[i], LOW);
    digitalWrite(ledGnd[(i + 1) % 4], HIGH);
    digitalWrite(ledGnd[(i + 2) % 4], HIGH);
    digitalWrite(ledGnd[(i + 3) % 4], HIGH);
    delay(6);
  }
}

int countdown = 0, safe = 0, score = 100;

void calc(int s) {
  for ( int i = 3; i >= 0; i--) {
    ledInt[i] = s % 10;
    s /= 10;
  }
}

void test() {
  if (digitalRead(buttonPin) == HIGH ) {
    if (countdown > 0) {
      countdown = 0;
      safe = 10;
      score += 10;
    }
    else if (safe < 0) {
      safe = 10;
      score -= 10;
    }
  }
}

void gg() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}

int loopN = 0, loopSec = 0;
void loop() {
  if (loopN == 0) {
    digitalWrite(13, HIGH);
    loopN = (rand() % 301) + 50; // 50~350 loops
    countdown = 40;
  }
  else {
    digitalWrite(13, LOW);
  }
  test();
  if (loopSec == 50) score--, loopSec = 0;
  calc(score);
  scan();
  if (score < 0) gg(), exit(1);
  countdown--;
  safe--;
  loopN--;
  loopSec++;
}
