#include <Wire.h>

#include <Adafruit_RGBLCDShield.h>
int ar(int pin){
  return analogRead(pin);
}
void log(String message);
#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\expander.ih"

using namespace std;


String nDigit(int i, size_t digits, char pad = '0') {
  if (digits == 1) {
    return String(i);
  }
  return i >= pow(10, digits - 1) ? String(i) : String(pad) + nDigit(i, digits - 1, pad);
}

String doubleDigit(int i) {
  return nDigit(i, 2);
}

void log(String message) {
  static Adafruit_RGBLCDShield lcd;
  lcd.begin(16, 2); // This contains Wire.begin()
  lcd.setCursor(0, 0);
  lcd.print(message + String(" "));
  delay(1000);
}

void setup() {
  Serial.begin(9600);
}
void loop() {
  IOExpander expa(0x21, 0x22);
  for(size_t i = 0; i != 2; ++i) {
    uint8_t pressed(expa.analogReadExt(i));
    log(String(i) + " is " + String(pressed));
    delay(500);
  }
}
