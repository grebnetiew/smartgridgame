#include <stdio.h>
#include <Arduino.h>
#include "expander.h"

const int LAKE_MAX    = 500; // Lake can hold this much power
const int COAL_MAX    = 50;  // Maximum power plant gen
const int SENSITIVITY = 5;   // City led turns yellow at this deficit/surplus
const int LINK_MAX    = 30;  // Max. power transport over link

struct CityState {
    // City uses a fixed graph with 4 vertices
    // and links (0,1), (0,2), (1,2), (1,3), (2,3)
    size_t  d_time = 0;
    int     d_link_delta[5] = {10, 0, 0, 0, -10};
    int     d_city_usage[4] = {10, 10, 10, 10};
    int     d_city_supply[4] = {10, 10, 10, 10};
    int     d_solar_power = 0;             // to city 3
    int     d_coal_power = COAL_MAX / 2;   // to city 0
    int     d_lake_contents = 0;
    double  d_price = 1.0;
    int     d_score = 0;

    void tick();
    void processButton(size_t btn);
    int  base_usage(int delta = 0) const;
    int  solar_power() const;
    void debugPrint() const;
};

struct BoardState {
    int ledCity[4];
    int ledCoal = 0;
    int ledLake = 0;
    bool buttonPressed[14]; // Prevent double presses

    IOExpander &exp;

    BoardState(IOExpander &e);
    void setLeds(CityState const &state);
    void updateScores(CityState &state) const;
    void readButtons(CityState &state);
};
