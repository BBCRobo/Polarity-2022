#include <MotorController.h>
#include <Common.h>


MotorController::MotorController(){};


/*! @brief Initialises the motor controllers pins.*/
void MotorController::init(int pin1, int pin2, int pwmpin) {
    INPIN1 = pin1;
    INPIN2 = pin2;
    PWMPIN = pwmpin;
    pinMode(INPIN1, OUTPUT);
    pinMode(INPIN2, OUTPUT);
    pinMode(PWMPIN, OUTPUT);
}


/*! @brief Writes values to the pins depending on the speed and direction of each motor.*/
void MotorController::moveMotor(int speed) {
    if(speed > 0) {
        //Forward Speed
        digitalWrite(INPIN1, HIGH);
        digitalWrite(INPIN2, LOW);
        analogWrite(PWMPIN, abs(speed * 2.55));
    } else if(speed < 0) {
        //Backward Speed
        digitalWrite(INPIN1, LOW);
        digitalWrite(INPIN2, HIGH);
        analogWrite(PWMPIN, abs(speed * 2.55));
    } else {
        //Stop Moving
        digitalWrite(INPIN1, HIGH);
        digitalWrite(INPIN2, HIGH);
        analogWrite(PWMPIN, 0);
    }
}