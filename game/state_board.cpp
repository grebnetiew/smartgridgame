#include "state.h"

BoardState::BoardState(IOExpander &e):
    ledCity({2, 2, 2, 2}),
    exp(e)
{
    memset(buttonPressed, 0, sizeof(buttonPressed));
}

void BoardState::setLeds(CityState const &state) {
    // Update the city led states
    for (size_t i = 0; i != 4; ++i) {
        int delta = state.d_city_supply[i] - state.d_city_usage[i];
        ledCity[i] = (delta < -20) ? 0 :
                     (delta < -5)  ? 1 :
                     (delta <= 5)  ? 2 :
                     (delta <= 20) ? 3 : 4;
    }

    ledCoal = (state.d_coal_power <=   COAL_MAX/3) ? 0 :
              (state.d_coal_power <= 2*COAL_MAX/3) ? 1 : 2;

    ledLake = (state.d_lake_contents <= 0.3 * LAKE_MAX) ? 0 :
              (state.d_lake_contents <= 0.7 * LAKE_MAX) ? 1 : 2;

    // City 1 has leds 0-4, 2 has 5-9 etc ...15-19
    for (size_t i = 0; i != 5; ++i) {
        exp.set(i,      ledCity[0] == i);
        exp.set(i + 5,  ledCity[1] == i);
        exp.set(i + 10, ledCity[2] == i);
        exp.set(i + 15, ledCity[3] == i);
    }
    for (size_t i = 0; i != 3; ++i) {
        // leds 20-22 are for power production
        exp.set(i + 20, ledCoal == i);
        // leds 23-25 are for the lake
        exp.set(i + 23, ledLake == i);
    }
}

void BoardState::readButtons(CityState &state) {
    for (size_t i = 0; i != 14; ++i) {
        bool pressed = exp.digitalReadExt(i);
        if (pressed && !buttonPressed[i]) {
            state.processButton(i);
        }
        buttonPressed[i] = pressed;
    }
}
