#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd;


void setup() {
  lcd.begin(16, 2); // This contains Wire.begin()
}

void loop() {
  for (int i = 0; i != 26; ++i) {
    pinMode(i + 28, OUTPUT);
    digitalWrite(i + 28, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Led " + String(i) + " ");
    delay(1000);
    digitalWrite(i + 28, LOW);
  }
}
