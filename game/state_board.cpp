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
        ledCity[i] = (delta < -4 * SENSITIVITY) ? 0 :
                     (delta < -1 * SENSITIVITY) ? 1 :
                     (delta <=     SENSITIVITY) ? 2 :
                     (delta <= 4 * SENSITIVITY) ? 3 : 4;
    }

    ledCoal = (state.d_coal_power <= COAL_MIN +     COAL_ADD / 3) ? 0 :
              (state.d_coal_power <= COAL_MIN + 2 * COAL_ADD / 3) ? 1 : 2;

    ledLake = (state.d_lake_contents <= 0.3 * LAKE_MAX) ? 0 :
              (state.d_lake_contents <= 0.7 * LAKE_MAX) ? 1 : 2;

    // City 1 has leds 0-4, 2 has 5-9 etc ...15-19
    for (int i = 0; i != 5; ++i) {
        exp.set(i,      ledCity[0] == i);
        exp.set(i + 5,  ledCity[1] == i);
        exp.set(i + 10, ledCity[2] == i);
        exp.set(i + 15, ledCity[3] == i);
    }
    for (int i = 0; i != 3; ++i) {
        // leds 20-22 are for power production
        exp.set(i + 20, ledCoal == i);
        // leds 23-25 are for the lake
        exp.set(i + 23, ledLake == i);
    }
    // If the lake is being filled or drained, blink the upper or lower led
    if(state.d_lake_delta > 3) {
      exp.set(25, state.d_time % 2);
    }
    if(state.d_lake_delta < -3) {
      exp.set(23, state.d_time % 2);
    }
}

void BoardState::updateScores(CityState &state) const {
  state.d_score += (ledCoal == 1 ? 5 : -10) + 
    static_cast<int>(10 * (1 - state.d_price)) +
    (ledCity[0] == 2 ? 3 : (ledCity[0] == 1 || ledCity[0] == 3) ? -1 : -5) +
    (ledCity[1] == 2 ? 3 : (ledCity[1] == 1 || ledCity[1] == 3) ? -1 : -5) +
    (ledCity[2] == 2 ? 3 : (ledCity[2] == 1 || ledCity[2] == 3) ? -1 : -5) +
    (ledCity[3] == 2 ? 3 : (ledCity[3] == 1 || ledCity[3] == 3) ? -1 : -5);
}

void BoardState::readAndProcessButtons(CityState &state) {
    for (size_t i = 0; i != 14; ++i) {
        bool pressed = exp.digitalReadExt(i);
        if (pressed && !buttonPressed[i]) {
            state.processButton(i);
        }
        buttonPressed[i] = pressed;
    }
}

void BoardState::resetIfKeyPressed(CityState &state) {
  for (size_t i = 0; i != 14; ++i) {
    if (exp.digitalReadExt(i)) {
      state.init();
    }
  }
}

