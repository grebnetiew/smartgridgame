// Fake IO expander
#ifndef _EXPANDER_H_
#define _EXPANDER_H_

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

#endif
