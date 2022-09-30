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
    int slow_speed_change(int target_speed, int target_angle);
    int slow_angle_change(int target_angle);
    void moveMotors(double angle, double rotation, int speed);
private:
    double motorAngles4[4] = {45, 315, 135, 225};
    unsigned long last_acceleration;
    unsigned long last_angle_acceleration;
    double acceleration_rate = 0.00001;
    float current_speed;
    float current_angle;
    MotorController motorA = MotorController();
    MotorController motorB = MotorController();
    MotorController motorC = MotorController();
    MotorController motorD = MotorController();
};

#endif