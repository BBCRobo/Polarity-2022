#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Pins.h>
#include <Arduino.h>

class MotorController {
public:
    MotorController();
    void init(int pin1, int pin2, int pwmpin);
    void moveMotor(int speed);
private:
    int INPIN1, INPIN2, PWMPIN;

};

#endif