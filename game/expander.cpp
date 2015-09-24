#include "expander.h"

IOExpander::IOExpander()
{
    // Due pins 0 and 1 are comms for the shield
    // Analog inputs are mapped to Due pins 14 till 27
    for (size_t i = 14; i != 28; ++i) {
        pinMode(i, INPUT);
    }
    // Digital outputs are mapped to Due pins 28 onwards
    for (size_t i = 28; i != 54; ++i) {
        pinMode(i, OUTPUT);
    }
}
