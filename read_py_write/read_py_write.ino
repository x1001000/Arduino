char s[] = "";
int i = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) s[i++] = Serial.read();
  if (Serial.available() == 0 && i != 0) {
    for (int j = 0; j < i; j++)
      Serial.write(s[j] > 64 && s[j] < 91 ? char(s[j] + 32) : s[j]);
    i = 0;
  }
}
