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
const size_t MIN_WRAP = 12;

void setup() {
  Serial.begin(9600);
  
  // Arduino setup phase
  pinMode(A0, INPUT);
  lcd.begin(16, 2); // This contains Wire.begin()

  // Euro glyph
  static uint8_t euro[8] = {7,8,30,8,30,8,7,0};
  lcd.createChar(0, euro);
}

void loop() {
  board.readButtons(state);
  if(++minutes == MIN_WRAP) {
    minutes = 0;
    state.tick();
    board.setLeds(state);
    updateScores();
    setLCD();
  }
}

void updateScores() {
  state.d_score += (board.ledCoal == 1 ? 20 : 0) +
    (board.ledCity[0] == 2 ? 3 : (board.ledCity[0] == 1 || board.ledCity[0] == 3) ? -1 : -5) +
    (board.ledCity[1] == 2 ? 3 : (board.ledCity[1] == 1 || board.ledCity[1] == 3) ? -1 : -5) +
    (board.ledCity[2] == 2 ? 3 : (board.ledCity[2] == 1 || board.ledCity[2] == 3) ? -1 : -5) +
    (board.ledCity[3] == 2 ? 3 : (board.ledCity[3] == 1 || board.ledCity[3] == 3) ? -1 : -5);
}

void setLCD() {  
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

