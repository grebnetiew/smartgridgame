#include "expander.h"

void IOExpander::init() const
{
    // Due pins 0 and 1 are comms for the shield
    // (Disused expander's) analog inputs are mapped to Due pins 14 till 27
    for (size_t i = 14; i != 28; ++i) {
        pinMode(i, INPUT);
    }
    // Digital outputs are mapped to Due pins 28 onwards
    for (size_t i = 28; i != 54; ++i) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
}

// Pins 20 and 21 are mapped to analog 8 and 9
// Pins 14 and 15 are mapped to analog 10 and 11
// This is because the current setup does not reliably read those
// buttons from digital pins, but it works fine from analog pins.
// (We do not know what causes this fault, and thus must deal with it)
bool IOExpander::digitalReadExt(uint8_t pin) const {
    if (pin == 6 || pin == 7) {
        return analogRead(pin - 6 + 8) > 500;
    }
    if (pin == 0 || pin == 1) {
        return analogRead(pin - 0 + 10) > 500;
    }
    return digitalRead(pin + 14) == HIGH;
}

