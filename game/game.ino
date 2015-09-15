#include <Wire.h>

// Made for use with Arduino
// todo: buttons

#include <Adafruit_RGBLCDShield.h>
#include "c:\Users\p250644\Documents\PhD\etc\festival-grrn\game\expander.ih"

using namespace std;

void log(String message) {}

String nDigit(int i, size_t digits, char pad = '0') {
  if (digits == 1) {
    return String(i);
  }
  return i >= pow(10, digits - 1) ? String(i) : String(pad) + nDigit(i, digits - 1, pad);
}

String doubleDigit(int i) {
  return nDigit(i, 2);
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
static uint8_t euro[8] = {7,8,30,8,30,8,7,0};

void setup() {
  v = new Vars();
  // Arduino setup phase
  pinMode(0, INPUT);
  Serial.begin(9600);
  v->lcd.begin(16, 2); // This contains Wire.begin()
  v->lcd.createChar(0, euro);
}

void loop() {
  if(++minutes == 12) {
    minutes = 0;
    v->state.tick();
    v->board.setLeds(v->state);
    updateScores();
  }
  // v->board.readButtons(v->state);
  setLCD();
}

void updateScores() {
  BoardState &board(v->board);
  v->state.d_score += (board.ledCoal == 1 ? 20 : 0) +
    (board.ledCity[0] == 2 ? 3 : (board.ledCity[0] == 1 || board.ledCity[0] == 3) ? -1 : -5) +
    (board.ledCity[1] == 2 ? 3 : (board.ledCity[1] == 1 || board.ledCity[1] == 3) ? -1 : -5) +
    (board.ledCity[2] == 2 ? 3 : (board.ledCity[2] == 1 || board.ledCity[2] == 3) ? -1 : -5) +
    (board.ledCity[3] == 2 ? 3 : (board.ledCity[3] == 1 || board.ledCity[3] == 3) ? -1 : -5);
}

void setLCD() {  
  // Print tijd
  v->lcd.setCursor(11, 0);
  v->lcd.print(doubleDigit(v->state.d_time / 10));
  v->lcd.setCursor(13, 0);
  v->lcd.print(":");
  v->lcd.print(doubleDigit(v->state.d_time % 10 * 6 + minutes / 2)); // Does it make sense? No. Don't care though :)

  // Print prijs
  v->lcd.setCursor(0, 0);
  v->lcd.write(0);
  v->lcd.print(" ");
  v->lcd.print(v->state.d_price);

  // Print score
  v->lcd.setCursor(0, 1);
  v->lcd.print("Score:     ");
  v->lcd.print(nDigit(v->state.d_score, 5, ' '));
}

