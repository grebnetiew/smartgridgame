// Made for use with Arduino
// still wanted: battery in one of the cities (lake?)
// : comms with leds
// : score keeping on an lcd

#include <string>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include "expander.ih"
using namespace std;

void log(string message) {
    // Nothing happens
}

#include "state.ih"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
IOExpander expander       = IOExpander();

CityState state;
BoardState board;
size_t minutes = 0;

void setup() {
    // Arduino setup phase
    Serial.begin(9600);
    lcd.begin(16, 2); // This contains Wire.begin()
}

void loop() {
    if(++minutes == 60) {
        minutes = 0;
        state.tick();
        board.setLeds(state);
    }
    setLCD();
}

void updateScores() {
    state.d_score += 
        (board.ledCity[0] == 2 ? 100 : (board.ledCity[0] == 1 || board.ledCity[0] == 3) ? 20 : 0) +
        (board.ledCity[1] == 2 ? 100 : (board.ledCity[1] == 1 || board.ledCity[1] == 3) ? 20 : 0) +
        (board.ledCity[2] == 2 ? 100 : (board.ledCity[2] == 1 || board.ledCity[2] == 3) ? 20 : 0) +
        (board.ledCity[3] == 2 ? 100 : (board.ledCity[3] == 1 || board.ledCity[3] == 3) ? 20 : 0) +
        600 - board.ledCoal*300;
}

void setLCD() {
    updateScores();
    lcd.setCursor(0, 0);
    lcd.print("Tijd:    :");
    lcd.setCursor(0, 1);
    lcd.print("Prijs:");

    lcd.setCursor(7, 0);
    lcd.print(state.d_time / 10);
    lcd.setCursor(10, 0);
    lcd.print(state.d_time * 6 + minutes / 10); // Does it make sense? No. Don't care though :)

    lcd.setCursor(7, 1);
    lcd.print(state.d_price);
}
