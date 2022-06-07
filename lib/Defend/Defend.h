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
    double correction(double hpid, bool dalive, bool defendangle);
    double yup(double defenddistance);
    double xleft(double straight);
    double power(bool dalive, bool balive, double straight, double defenddistance, double balldist);
    double defence(double defenddistance, double straight);
    double orbit(double straight, double balldist);
    double angle(double straight, double defenddistance, double balldist, bool dalive, bool balive);
};
#endif