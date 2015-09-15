#include <Wire.h>

// Made for use with Arduino
// todo: buttons

#include <Adafruit_RGBLCDShield.h>
#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\expander.ih"

using namespace std;

void log(String message) {}

String doubleDigit(int i) {
  static String zero('0');
  return i > 9 ? String(i) : String(0) + String(i);
}

#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\state.ih"

class Vars {
  public:
    Adafruit_RGBLCDShield lcd;
    IOExpander expander;
    CityState state;
    BoardState board;
    Vars() : lcd(), expander(), state(), board(expander) {}
};

Vars *v;

size_t minutes = 0;

void setup() {
  v = new Vars();
  // Arduino setup phase
  pinMode(0, INPUT);
  Serial.begin(9600);
  v->lcd.begin(16, 2); // This contains Wire.begin()
}

void loop() {
  if(++minutes == 12) {
    minutes = 0;
    v->state.tick();
    v->board.setLeds(v->state);
  }
  // v->board.readButtons(v->state);
  setLCD();
}

void updateScores() {
  BoardState &board(v->board);
  v->state.d_score += 600 - board.ledCoal*300 +
    (board.ledCity[0] == 2 ? 100 : (board.ledCity[0] == 1 || board.ledCity[0] == 3) ? 20 : 0) +
    (board.ledCity[1] == 2 ? 100 : (board.ledCity[1] == 1 || board.ledCity[1] == 3) ? 20 : 0) +
    (board.ledCity[2] == 2 ? 100 : (board.ledCity[2] == 1 || board.ledCity[2] == 3) ? 20 : 0) +
    (board.ledCity[3] == 2 ? 100 : (board.ledCity[3] == 1 || board.ledCity[3] == 3) ? 20 : 0);
}

void setLCD() {
  updateScores();
  v->lcd.setCursor(0, 0);
  v->lcd.print("Tijd:");
  v->lcd.setCursor(0, 1);
  v->lcd.print("Prijs:");

  v->lcd.setCursor(7, 0);
  v->lcd.print(doubleDigit(v->state.d_time / 10));
  v->lcd.setCursor(9, 0);
  v->lcd.print(":");
  v->lcd.print(doubleDigit(v->state.d_time % 10 * 6 + minutes / 2)); // Does it make sense? No. Don't care though :)

  v->lcd.setCursor(7, 1);
  v->lcd.print(v->state.d_price);
}

