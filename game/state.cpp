#include "state.h"

CityState::CityState() {
    init();
}

void CityState::init() {
    d_time          = 0;
    d_link_delta[0] = 10;
    d_link_delta[1] = 0;
    d_link_delta[2] = 0;
    d_link_delta[3] = 0;
    d_link_delta[4] = -10;
    for (size_t i = 0; i != 4; ++i) {
        d_city_usage[i]  = 10;
        d_city_supply[i] = 10;
    }
    d_solar_power   = 0;             // to city 3
    d_coal_power    = COAL_MIN + COAL_ADD / 2;   // to city 0
    d_lake_contents = 0;
    d_lake_delta    = 0;
    d_price         = 1.0;
    d_score         = 0;
}

void CityState::tick() {
    if(d_time == 240) {
        return;
    }
    ++d_time;
    d_solar_power = solar_power();

    d_city_usage[0] = base_usage(0)  / d_price * 0.7;
    d_city_usage[1] = base_usage(-1) / d_price * 1.3;
    d_city_usage[2] = base_usage(0)  / d_price * 1.1;
    d_city_usage[3] = base_usage(2)  / d_price * 0.9;

    d_city_supply[0] = d_coal_power - d_link_delta[0] - d_link_delta[1];
    d_city_supply[1] = d_link_delta[0] - d_link_delta[2] - d_link_delta[3];
    d_city_supply[2] = d_link_delta[1] + d_link_delta[2] - d_link_delta[4];
    d_city_supply[3] = d_solar_power + d_link_delta[3] + d_link_delta[4];
    int previous_lake_contents = d_lake_contents;
    // Deal with the lake
    int surplus1 = d_city_supply[1] - d_city_usage[1]; // Positive = surplus
    if (surplus1 < 0) { // take from lake
        if (-surplus1 > d_lake_contents) { // not enough there
            d_city_usage[1] -= d_lake_contents;
            d_lake_contents = 0;
        } else {
            d_lake_contents += surplus1;
            d_city_usage[1] = d_city_supply[1];
        }
    } else { // surplus2 > 0, Give to lake
        if (surplus1 + d_lake_contents > LAKE_MAX) { // it will be full
            d_city_usage[1] += 500 - d_lake_contents;
            d_lake_contents = LAKE_MAX;
        } else {
            d_lake_contents += surplus1;
            d_city_usage[1] = d_city_supply[1];
        }
    }
    d_lake_delta = d_lake_contents - previous_lake_contents;
    debugPrint();
}

extern IOExpander expander;

void CityState::debugPrint() const {
    char output[40];
    Serial.println("Tick: " + String(d_time));
    sprintf(output, "%d (%d) %d (%d) %d", d_city_supply[0], d_link_delta[0], d_city_supply[1], d_link_delta[3], d_city_supply[3]);
    Serial.println(output);
    sprintf(output, "   (%d) (%d)  (%d)", d_link_delta[1], d_link_delta[2], d_link_delta[4]);
    Serial.println(output);
    sprintf(output, "            %d    ", d_city_supply[2]);
    Serial.println(output);
    Serial.println("Lake: " + String(d_lake_contents));
    Serial.println("Power (solar + plant): " + String(d_solar_power) + " " + String(d_coal_power));
    Serial.println("Button map: " + 
      String(expander.digitalReadExt(0)) + " " +
      String(expander.digitalReadExt(1)) + " " +
      String(expander.digitalReadExt(2)) + " " +
      String(expander.digitalReadExt(3)) + " " +
      String(expander.digitalReadExt(4)) + " " +
      String(expander.digitalReadExt(5)) + " " +
      String(expander.digitalReadExt(6)) + " " +
      String(expander.digitalReadExt(7)) + " " +
      String(expander.digitalReadExt(8)) + " " +
      String(expander.digitalReadExt(9)) + " " +
      String(expander.digitalReadExt(10)) + " " +
      String(expander.digitalReadExt(11)) + " " +
      String(expander.digitalReadExt(12)) + " " +
      String(expander.digitalReadExt(13)));
}

static int button_mult[5] = {1, -1, 1, -1, 1};

void CityState::processButton(size_t btn) {
    // 0 (-) and 1 (+) are for link 1, and so on (link 5 has 8, 9)
    if (btn < 10 && btn % 2 == 0) {
        d_link_delta[btn / 2] -= LINK_GRANULARITY * button_mult[btn/2];
        if (d_link_delta[btn / 2] < -LINK_MAX)
            d_link_delta[btn / 2] = -LINK_MAX;
        if (d_link_delta[btn / 2] >  LINK_MAX)
            d_link_delta[btn / 2] =  LINK_MAX;
    }
    if (btn < 10 && btn % 2 == 1) {
        d_link_delta[btn / 2] += LINK_GRANULARITY * button_mult[btn/2];
        if (d_link_delta[btn / 2] >  LINK_MAX)
            d_link_delta[btn / 2] =  LINK_MAX;
        if (d_link_delta[btn / 2] < -LINK_MAX)
            d_link_delta[btn / 2] = -LINK_MAX;
    }

    // 10 (-), 11 are the production
    if (btn == 10 && d_coal_power > COAL_MIN) {
        d_coal_power -= COAL_ADD / 2;
    }
    if (btn == 11 && d_coal_power < COAL_MIN + COAL_ADD) {
        d_coal_power += COAL_ADD / 2;
    }

    // 12 (-), 13 are the price
    if (btn == 12 && d_price > 0.7) {
        d_price -= .1;
    }
    if (btn == 13 && d_price < 1.4) { 
        d_price += .1;
    }
}

static int function_usage[24] = {13, 10, 9, 7, 5, 5, 8, 12, 13, 12,
    12, 11, 10, 10, 9, 9, 10, 12, 16, 17, 17, 16, 14, 14};
inline int CityState::base_usage(int delta) const {
    return function_usage[(delta / 10 + delta + 24) % 24];
}

static int function_power[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 6, 8, 9, 9, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0};
inline int CityState::solar_power() const {
    return function_power[d_time / 10];
}
