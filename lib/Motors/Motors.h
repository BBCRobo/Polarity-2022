#ifndef MOTORS_H
#define MOTORS_H

#include <Pins.h>
#include <math.h>
#include <MotorController.h>
#include <Common.h>
#include <Arduino.h>

class Motors {
public:
    Motors();
    void init();
    void moveMotors(double angle, double rotation, int speed);
private:
    double motorAngles4[4] = {45, 315, 135, 225};
    MotorController motorA = MotorController();
    MotorController motorB = MotorController();
    MotorController motorC = MotorController();
    MotorController motorD = MotorController();
};

#endif