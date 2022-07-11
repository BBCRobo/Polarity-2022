#ifndef LRF_H
#define LRF_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <VL53L1X.h>

class LRF {
public:
    LRF() {};
    void init();
    void enable(int lrf_no);
    void read_one(int lrf_no);
    void read_all();
    bool lrf_init[4] = {false, true, true, true};
    double lrf_values[4] = {0,0,0,0};
private:
    const uint8_t lrf_pins[4] = {LRF_1, LRF_2, LRF_3, LRF_4};
};
#endif