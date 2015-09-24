// Fake IO expander

// The layout is as follows (digital io):
//  0- 7 PORTB of U2
//  8-15 PORTA of U2
// 16-23 PORTB of U4
// 24-27 PORTA of U4 (only connectors 4-7, the others select analogread addr)
// The analog pins are input only, 16 times. These will be buttons. 

#include <stdint.h>
#include <Arduino.h>

class IOExpander {
public:
    IOExpander();
    void set(size_t pin, bool state);
    bool digitalReadExt(uint8_t pin);
    void update();
};

inline void IOExpander::set(size_t pin, bool newState) {
    digitalWrite(pin + 28, newState ? HIGH : LOW);
}

inline void IOExpander::update() {}

inline bool IOExpander::digitalReadExt(uint8_t pin) {
    return digitalRead(pin + 14) == HIGH;
}

IOExpander::IOExpander()
{
    // Due pins 0 and 1 are comms for the shield
    // Analog inputs are mapped to Due pins 14 till 27
    for (size_t i = 14; i != 28; ++i)
        pinMode(i, INPUT);
    // Digital outputs are mapped to Due pins 28 onwards
    for (size_t i = 28; i != 54; ++i)
        pinMode(i, OUTPUT);
}