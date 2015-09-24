#include "state.h"

void CityState::tick() {
    if(++d_time == 240) {
        d_time = 0;
        d_score = 0;
    }
    d_solar_power = solar_power();

    d_city_usage[0] = base_usage(0)  / d_price * 0.7;
    d_city_usage[1] = base_usage(-1) / d_price * 1.3;
    d_city_usage[2] = base_usage(0)  / d_price * 1.1;
    d_city_usage[3] = base_usage(2)  / d_price * 0.9;

    d_city_supply[0] = d_coal_power - d_link_delta[0] - d_link_delta[1];
    d_city_supply[1] = d_link_delta[0] - d_link_delta[2] - d_link_delta[3];
    d_city_supply[2] = d_link_delta[1] + d_link_delta[2] - d_link_delta[4];
    d_city_supply[3] = d_solar_power + d_link_delta[3] + d_link_delta[4];

    // Deal with the lake
    int surplus2 = d_city_supply[2] - d_city_usage[2]; // Positive = surplus
    if (surplus2 < 0) { // take from lake
        if (-surplus2 > d_lake_contents) { // not enough there
            d_city_usage[2] -= d_lake_contents;
            d_lake_contents = 0;
        } else {
            d_lake_contents += surplus2;
            d_city_usage[2] = d_city_supply[2];
        }
    } else { // surplus2 > 0, Give to lake
        if (surplus2 + d_lake_contents > LAKE_MAX) { // it will be full
            d_city_usage[2] += 500 - d_lake_contents;
            d_lake_contents = LAKE_MAX;
        } else {
            d_lake_contents += surplus2;
            d_city_usage[2] = d_city_supply[2];
        }
    }

    char output[40];
    Serial.println("Tick: " + String(d_time));
    sprintf(output, "%d (%d) %d (%d) %d", d_city_supply[0], d_link_delta[0], d_city_supply[1], d_link_delta[3], d_city_supply[3]);
    Serial.println(output);
    sprintf(output, "   (%d) (%d)  (%d)", d_link_delta[1], d_link_delta[2], d_link_delta[4]);
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
        d_link_delta[btn / 2] -= LINK_MAX / 3;
        if (d_link_delta[btn / 2] < -LINK_MAX)
            d_link_delta[btn / 2] = -LINK_MAX;
    }
    if (btn < 10 && btn % 2 == 1) {
        d_link_delta[btn / 2] += LINK_MAX / 3;
        if (d_link_delta[btn / 2] > LINK_MAX)
            d_link_delta[btn / 2] = LINK_MAX;
    }

    // 10 (-), 11 are the production
    if (btn == 10 && d_coal_power > 0) {
        d_coal_power -= COAL_MAX / 2;
    }
    if (btn == 11 && d_coal_power < COAL_MAX) {
        d_coal_power += COAL_MAX / 2;
    }

    // 12 (-), 13 are the price
    if (btn == 12 && d_price > 0.7) {
        d_price -= .1;
    }
    if (btn == 13 && d_price < 1.4) { 
        d_price += .1;
    }
}

static int function_usage[24] = {13, 12, 11, 9, 7, 7, 8, 12, 13, 12,
    12, 11, 10, 10, 9, 9, 10, 12, 16, 17, 17, 16, 14, 14};
inline int CityState::base_usage(int delta) {
    return function_usage[(delta / 10 + delta + 24) % 24];
}

static int function_power[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 6, 8, 9, 9, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0};
inline int CityState::solar_power() {
    return function_power[d_time / 10];
}
