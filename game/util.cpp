#include "util.h"
#include <Adafruit_RGBLCDShield.h>

extern Adafruit_RGBLCDShield lcd;

void log(String message) {
  lcd.setCursor(0, 0);
  lcd.print(message + String(" "));
  delay(1000);
}

String nDigit(int i, size_t digits, char pad) {
  if (i < 0) {
    return "-" + nDigit(-i, digits, pad);
  }
  if (digits == 1) {
    return String(i);
  }
  return i >= pow(10, digits - 1) ? String(i) : String(pad) + nDigit(i, digits - 1, pad);
}

String doubleDigit(int i) {
  return nDigit(i, 2);
}
