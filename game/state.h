#include <stdio.h>
#include <Arduino.h>
#include "expander.h"

const int LAKE_MAX = 500;

class CityState {
    // City uses a fixed graph with 4 vertices
    // and links (0,1), (0,2), (1,2), (1,3), (2,3)
public: // yeah yeah
    size_t  d_time = 0;
    int     d_link_delta[5] = {10, 0, 0, 0, -10};
    int     d_city_usage[4] = {10, 10, 10, 10};
    int     d_city_supply[4] = {10, 10, 10, 10};
    size_t  d_solar_power = 20;  // to city 3
    size_t  d_coal_power = 20;   // to city 0
    size_t  d_lake_contents = 0;
    double  d_price = 1.0;
    size_t  d_score = 0;

    void tick();
    void processButton(size_t btn);
    size_t base_usage();
    size_t solar_power();
};

class BoardState {
public:
    int ledCity[4];
    int ledCoal = 0;
    int ledLake = 0;
    bool buttonPressed[14]; // Prevent double presses

    IOExpander &exp;

    BoardState(IOExpander &e);
    void setLeds(CityState const &state);
    void readButtons(CityState &state);
};
