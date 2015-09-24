#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\util.h"
#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\expander.ih"
#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\state.ih"
using namespace std;
 
// It seems problematic to have global class-type objects, so we just make a pointer
// to a collection of objects.
Adafruit_RGBLCDShield lcd;
IOExpander expander;
CityState state;
BoardState board(expander);

#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\util.ih"

size_t minutes = 0;
#define MIN_WRAP 12

void setup() {
  Serial.begin(9600);
  
  // Arduino setup phase
  pinMode(A0, INPUT);
  lcd.begin(16, 2); // This contains Wire.begin()

  // Euro glyph
  static uint8_t euro[8] = {7,8,30,8,30,8,7,0};
  lcd.createChar(0, euro);
  
  log(" "); // Critical for correct activation. Don't ask.
}

void loop() {
  if(++minutes == MIN_WRAP) {
    minutes = 0;
    state.tick();
    board.setLeds(state);
    updateScores();
  }
  board.readButtons(state);
  setLCD();
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

