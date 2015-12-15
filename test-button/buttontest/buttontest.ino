#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd;


void setup() {
  lcd.begin(16, 2); // This contains Wire.begin()
  for (int i = 14; i != 28; ++i) {
    pinMode(i,INPUT);
  }
}

void loop() {
  String s("");
  for (int i = 14; i != 28; ++i) {
    if (digitalRead(i) == HIGH) {
      s += String(i) + " ";
    }
  }
  lcd.setCursor(0, 0);
  lcd.print(s + "                        ");
  lcd.setCursor(0, 1);
  lcd.print(String(analogRead(8)) + " " + String(analogRead(9)) + " " + String(analogRead(10)) + " " + String(analogRead(11)) + "      ");
  delay(1000);
}
