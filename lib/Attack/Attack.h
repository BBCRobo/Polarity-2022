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
    double correction(double hpid, double attackangle, bool aalive, double balldist);
    double orbit(double straight, double balldist);
    double angle(bool aalive, bool balive, bool dalive, double attackdistance, double defendangle, double balldist, double straight);
};

#endif