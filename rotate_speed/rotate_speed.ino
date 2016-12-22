volatile int count = 0;
unsigned long lastmillis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), plusOne, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastmillis == 1000) {
    detachInterrupt(digitalPinToInterrupt(2));
    Serial.print("Speed = ");
    Serial.print(count / 2.0);
    Serial.println(" Hz");
    count = 0;
    lastmillis = millis();
    attachInterrupt(digitalPinToInterrupt(2), plusOne, FALLING);
  }
}

void plusOne() {
  count++;
}
