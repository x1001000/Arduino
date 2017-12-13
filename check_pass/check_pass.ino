int enter[4] = {0, 0, 0, 0};
int pass[4] = {'5', '5', '6', '6'};
bool bingo = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter pass:");
}

void loop() {
  if (Serial.available() > 0) {

    for (int i = 1; i < 4; i++)
      enter[i - 1] = enter[i];
    enter[3] = Serial.read();

    for (int i = 0; i < 4; i++) {
      if (enter[i] != pass[i])
      {
        bingo = false;
        break;
      }
      bingo = true;
    }
    if (bingo)
      Serial.println("Bingo");
  }
}
