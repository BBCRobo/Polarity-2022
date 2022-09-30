#ifndef ATTACK_H
#define ATTACK_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <PID.h>
#include <Camera.h>
#include <math.h>

class Attack {
public:
    Attack() {};
    double orbit(double straight, double balldist, double attackdistance);
    double power(double straight, double balldist, bool balive);
    double angle(bool balive, double balldist, double straight, int pos_x, int pos_y, double attackdistance);
    double circle();
private:
    double goalangle;
    unsigned long last_circle_accl = -2;
    double acceleration_rate = 0.001;
    float current_angle;
    int neutral_point_pos[4][2] = {{610,700},{1440,700},{610,960},{1440,960}};
    bool checked_neutral_point[4] = {false, false, false, false};
};

#endif