#include <stdio.h>

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

void CityState::tick() {
    if(++d_time == 240) {
        d_time = 0;
        d_score = 0;
    }
    d_solar_power = solar_power();

    d_city_usage[0] = base_usage() / d_price * 0.7;
    d_city_usage[1] = base_usage() / d_price * 1.3;
    d_city_usage[2] = base_usage() / d_price * 1.1;
    d_city_usage[3] = base_usage() / d_price * 0.9;

    d_city_supply[0] = d_coal_power - d_link_delta[0] - d_link_delta[1];
    d_city_supply[1] = d_link_delta[0] - d_link_delta[2] - d_link_delta[3];
    d_city_supply[2] = d_link_delta[1] + d_link_delta[2] - d_link_delta[4];
    d_city_supply[3] = d_solar_power + d_link_delta[3] + d_link_delta[4];

    // Deal with the lake
    if (d_city_supply[2] < d_city_usage[2]) { // take from lake
        if (d_city_usage[2] - d_city_supply[2] > d_lake_contents) { // not enough there
            d_city_supply[2] += d_lake_contents;
            d_lake_contents = 0;
        } else {
            d_lake_contents -= d_city_usage[2] - d_city_supply[2];
            d_city_supply[2] = d_city_usage[2];
        }
    } else { // Give to lake
        if (d_city_supply[2] - d_city_usage[2] + d_lake_contents > 500) { // it will be full
            d_city_supply[2] -= 500 - d_lake_contents;
            d_lake_contents = 500;
        } else {
            d_lake_contents += d_city_supply[2] - d_city_usage[2];
            d_city_supply[2] = d_city_usage[2];
        }
    }

    char output[30];
    Serial.println("Tick: " + String(d_time));
    sprintf(output, "%d (%d) %d (%d) %d", d_city_supply[0], d_link_delta[0],
        d_city_supply[1], d_link_delta[3], d_city_supply[3]);
    Serial.println(output);
    sprintf(output, "   (%d) (%d)  (%d)", d_link_delta[1], d_link_delta[2],
        d_link_delta[4]);
    Serial.println(output);
    sprintf(output, "            %d    ", d_city_supply[2]);
    Serial.println(output);
    Serial.println("Lake: " + String(d_lake_contents));
    Serial.println("Power (solar + plant): " + String(d_solar_power) 
        + " " + String(d_coal_power));
}

void CityState::processButton(size_t btn) {
    // 0 (-) and 1 (+) are for link 1, and so on (link 5 has 8, 9)
    if (btn < 10 && btn % 2 == 0) {
        d_link_delta[btn / 2] -= 10;
        if (d_link_delta[btn / 2] < -30)
            d_link_delta[btn / 2] = -30;
    }
    if (btn < 10 && btn % 2 == 1) {
        d_link_delta[btn / 2] += 10;
        if (d_link_delta[btn / 2] > 30)
            d_link_delta[btn / 2] = 30;
    }

    // 10 (-), 11 are the production
    if (btn == 10 && d_coal_power > 0) {
        d_coal_power -= 10;
    }
    if (btn == 11 && d_coal_power < 50) {
        d_coal_power += 10;
    }

    // 12 (-), 13 are the price
    if (btn == 12 && d_price > 0.7) {
        d_price -= .1;
    }
    if (btn == 13 && d_price < 1.4) { 
        d_price += .1;
    }
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

    ledCoal = (state.d_coal_power <= 20) ? 0 :
              (state.d_coal_power <= 40) ? 1 : 2;

    ledLake = (state.d_lake_contents <= 150) ? 0 :
              (state.d_lake_contents <= 350) ? 1 : 2;

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
    exp.update();
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