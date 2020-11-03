#include <LiquidCrystal.h>
#include <time.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int TODOS_COUNT = 7;
char todos[TODOS_COUNT][16] = { "Ice Cream", "Peach", "Apple", "Pistacia", "Orange", "Kiwi", "Potato Chips" };

const int buttonPin = 6;
const int speakerPin = 7;

int buttonState = 0;
int bannerState = 0;
unsigned long lastTimeButtonPressed = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  printBanner();
}

void printBanner() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("SNACK");
  lcd.setCursor(4, 1);
  lcd.print("SPINNER");
  bannerState = 1;
}

void printRandomTodo() {
  int i = random(TODOS_COUNT);
  lcd.print(todos[i]);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    lcd.clear();
    printRandomTodo();

    digitalWrite(speakerPin, HIGH);
    delay(50);
    digitalWrite(speakerPin, LOW);
    
    bannerState = 0;
    lastTimeButtonPressed = millis();
  }

  if (bannerState == 0 && millis() - lastTimeButtonPressed > 5000) {
    printBanner();
  }
}
