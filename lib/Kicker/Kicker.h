#ifndef KICKER_H
#define KICKER_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>

class Kicker {
public:
    Kicker() {};
    void init();
    void fire();
    bool check_LDR();
    int LDR_high = -1;
private:
    bool kicking = false;
    unsigned long last_kick;
};
#endif