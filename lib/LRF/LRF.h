#ifndef LRF_H
#define LRF_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <VL53L1X.h>

class Lrf {
public:
    Lrf() {};
    void init();
    double read(int lrf);
};
#endif