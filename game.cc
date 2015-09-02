// Made for use with Arduino
// still wanted: battery in one of the cities (lake?)
// : comms with leds
// : score keeping on an lcd
#include <string>
using namespace std;

void log(string message) {
    // Nothing happens
}

class CityState {
    // City uses a fixed graph with 4 vertices
    // and links (0,1), (0,2), (1,2), (1,3), (2,3)
public: // yeah yeah
    size_t  d_time = 0;
    int     d_link_delta[5] = {10, 0, 0, 0, -10};
    size_t  d_city_usage[4] = {10, 10, 10, 10};
    int     d_city_supply[4] = {10, 10, 10, 10};
    size_t  d_solar_power = 20; // to city 3
    size_t  d_coal_power = 20; // to city 0
    double  d_price = 1.0;
    size_t score = 0;

    void tick();
    size_t base_usage();
    size_t solar_power();
};

void CityState::tick() {
    if(++d_time == 240) {
        d_time = 0;
    }
    d_city_usage[0] = base_usage() * d_price * 0.7;
    d_city_usage[1] = base_usage() * d_price * 1.3;
    d_city_usage[2] = base_usage() * d_price * 1.1;
    d_city_usage[3] = base_usage() * d_price * 0.9;

    d_city_supply[0] = d_coal_power - d_link_delta[0] - d_link_delta[1];
    d_city_supply[1] = d_link_delta[0] - d_link_delta[2] - d_link_delta[3];
    d_city_supply[2] = d_link_delta[1] + d_link_delta[2] - d_link_delta[4];
    d_city_supply[3] = d_solar_power + d_link_delta[3] + d_link_delta[4];

    log("Tick " + to_string(d_time) + " complete");
}

size_t CityState::base_usage() {
    static size_t usage[24] = {13, 12, 11, 9, 7, 7, 8, 12, 13, 12,
        12, 11, 10, 10, 9, 9, 10, 12, 16, 17, 17, 16, 14, 14};
    return usage[d_time / 10];
}

size_t CityState::solar_power() {
    static size_t power[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 6, 8, 9, 9, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0};
    return power[d_time / 10];
}

struct BoardState {
    size_t ledCity[4] = {2, 2, 2, 2};
    size_t ledSolar = 0;
    size_t ledCoal = 0;
    size_t ledLake = 0;
}

CityState state;
BoardState board;
size_t minutes = 0;

void setup() {
    // Arduino setup phase
}

void loop() {
    if(++minutes == 60) {
        minutes = 0;
        state.tick();
        updateScores();
        updateLEDs();
    }
}

void updateScores() {

}

void updateLEDs() {

}