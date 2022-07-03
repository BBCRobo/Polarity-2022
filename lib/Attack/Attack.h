#ifndef ATTACK_H
#define ATTACK_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <PID.h>
#include <Camera.h>

class Attack {
public:
    Attack() {};
    double orbit(double straight, double balldist);
    double power(double straight, double balldist);
    double angle(bool balive, double balldist, double straight);
private:
    double goalangle;
};

#endif