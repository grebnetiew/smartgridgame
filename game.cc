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
        updateScores();
        updateLEDs();
    }
}

void updateScores() {

}

void updateLEDs() {

}