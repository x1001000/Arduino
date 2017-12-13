#include "pitches.h"

int melody[] = {
  NOTE_G3,NOTE_C4,NOTE_C4,NOTE_E4,NOTE_A4,NOTE_E4,NOTE_G4,
  NOTE_G4,NOTE_A4,NOTE_G4,NOTE_E4,NOTE_F4,NOTE_E4,NOTE_D4,
  NOTE_A3,NOTE_D4,NOTE_D4,NOTE_F4,NOTE_B4,NOTE_B4,NOTE_A4,NOTE_G4,
  NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_A3,NOTE_B3,NOTE_C4,NOTE_D4
};
int note = 0;

void setup() {
  pinMode(2, INPUT);
}

void loop() {
  while(digitalRead(2)==LOW) delay(1);
  while(digitalRead(2)==HIGH) tone(8, melody[note], 50);
  note++;
  note%=30;
}
