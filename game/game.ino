#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
Adafruit_RGBLCDShield lcd;

#include "expander.h"
#include "state.h"
#include "util.h"

// It seems problematic to have global class-type objects, so we just make a pointer
// to a collection of objects.
IOExpander expander;
CityState state;
BoardState board(expander);

size_t minutes = 0;
size_t highScore = 0;
const size_t MIN_WRAP = 8;

void setup() {
  expander.init();
  Serial.begin(9600);
  lcd.begin(16, 2); // This contains Wire.begin()
  
  // Euro glyph
  static uint8_t euro[8] = {7,8,30,8,30,8,7,0};
  lcd.createChar(0, euro);
}

void loop() {
  if (state.d_time == 240) {
    if (state.d_score > highScore) {
      highScore = state.d_score;
    }
    highscoreLCD();
    board.resetIfKeyPressed(state);
    return;
  }
  board.readAndProcessButtons(state);
  board.setLeds(state);
  if(++minutes == MIN_WRAP) {
    minutes = 0;
    state.tick();
    board.updateScores(state);
  }
  updateLCD();
}

void updateLCD() {  
  // Print tijd
  lcd.setCursor(11, 0);
  lcd.print(doubleDigit(state.d_time / 10));
  lcd.setCursor(13, 0);
  lcd.print(":");
  lcd.print(doubleDigit(state.d_time % 10 * 6 + 6 * minutes / MIN_WRAP)); // Does it make sense? No. Don't care though :)

  // Print prijs
  lcd.setCursor(0, 0);
  lcd.write(0); // euro
  lcd.print(" ");
  lcd.print(state.d_price);

  // Print score
  lcd.setCursor(0, 1);
  lcd.print("Score:     ");
  lcd.print(nDigit(state.d_score, 5, ' '));
}

void highscoreLCD() {  
  // Print score
  lcd.setCursor(0, 0);
  lcd.print("Score:     ");
  lcd.print(nDigit(state.d_score, 5, ' '));

  // Print score
  lcd.setCursor(0, 1);
  lcd.print("Highscore: ");
  lcd.print(nDigit(highScore, 5, ' '));
}

