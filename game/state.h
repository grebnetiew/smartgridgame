#include <stdio.h>
#include <Arduino.h>
#include "expander.h"

const int LAKE_MAX         = 500; // Lake can hold this much power
const int COAL_MIN         = 20;  // Minimum generated energy
const int COAL_ADD         = 50;  // Maximum power plant gen on top of MIN
const int SENSITIVITY      = 5;   // City led turns yellow at this deficit/surplus
const int LINK_MAX         = 100; // Max. power transport over link
const int LINK_GRANULARITY = 10;  // What gets added each time you press a button

struct CityState {
    // City uses a fixed graph with 4 vertices
    // and links (0,1), (0,2), (1,2), (1,3), (2,3)
    size_t  d_time;
    int     d_link_delta[5];
    int     d_city_usage[4];
    int     d_city_supply[4];
    int     d_solar_power;  // to city 3
    int     d_coal_power;   // to city 0
    int     d_lake_contents;
    int     d_lake_delta;
    double  d_price;
    int     d_score;

    CityState();
    void tick();
    void processButton(size_t btn);
    void init();
private:
    void debugPrint() const;
    int  base_usage(int delta = 0) const;
    int  solar_power() const;
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
    void readAndProcessButtons(CityState &state);
    void resetIfKeyPressed(CityState &state);
};
