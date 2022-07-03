#ifndef DEFEND_H
#define DEFEND_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <PID.h>
#include <Camera.h>

class Defend {
public:
    Defend() {};
    double verticalPID(double defenddistance);
    double horizontalPID(double straight);
    double power(bool dalive, bool balive, double straight, double defenddistance, double balldist);
    double defence(double defenddistance, double straight);
    double angle(double straight, double defenddistance, double balldist, bool dalive, bool balive);
};
#endif