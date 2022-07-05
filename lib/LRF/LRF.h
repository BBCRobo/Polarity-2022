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
    // double read(int lrf);
    double read(int lrf_no);
private:
    int lrf_pins[4] = {LRF_1, LRF_2, LRF_3, LRF_4};
};
#endif