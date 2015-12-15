// Fake IO expander
#ifndef _EXPANDER_H_
#define _EXPANDER_H_

#include <stdint.h>
#include <Arduino.h>

class IOExpander {
public:
    void init() const;
    void set(size_t pin, bool state);
    bool digitalReadExt(uint8_t pin) const;
};


inline void IOExpander::set(size_t pin, bool newState) {
    digitalWrite(pin + 28, newState ? HIGH : LOW);
}

#endif
