int pinBuzzer = 9;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
  long frequency = 100;
  tone(pinBuzzer, frequency);
  delay(1000);

  noTone(pinBuzzer);
  delay(2000);

}
