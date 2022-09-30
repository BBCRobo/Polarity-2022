#include <Motors.h>


Motors::Motors(){};


/*! @brief Initialises each motor.*/
void Motors::init() {
    motorA.init(INONE_FR, INTWO_FR, PWM_FR);
    motorB.init(INONE_FL, INTWO_FL, PWM_FL);
    motorC.init(INONE_BR, INTWO_BR, PWM_BR);
    motorD.init(INONE_BL, INTWO_BL, PWM_BL);
    last_acceleration = -1;
    last_angle_acceleration = -1;
    current_speed = 0;
}


int Motors::slow_speed_change(int target_speed, int target_angle) {
    unsigned long current_time = micros();
    if(last_acceleration == -1) {
        last_acceleration = micros();
        return 0;
    }
    if(abs(target_angle - current_angle) < 45 || (abs(target_angle - current_angle) > 180 && abs(target_angle - current_angle - 360) < 45)) {
        if(target_speed > current_speed) {
            float returning_speed = current_speed + acceleration_rate*(current_time - last_acceleration);
            current_speed = min(target_speed, returning_speed);
        } else {
            float returning_speed = current_speed - acceleration_rate*(current_time - last_acceleration);
            current_speed = max(target_speed, returning_speed);
        }
    } else {
        if(current_speed > 10) {
            float returning_speed = current_speed - acceleration_rate*(current_time - last_acceleration);
            current_speed = max(0, returning_speed);
        }
    }
    last_acceleration = micros();
    return int(current_speed);
}


int Motors::slow_angle_change(int target_angle) {
    unsigned long current_time = micros();
    if(last_angle_acceleration == -1 || target_angle == -1) {
        last_angle_acceleration = micros();
        return -1;
    }
    if(abs(target_angle - current_angle) < 45 || (abs(target_angle - current_angle) > 180 && abs(target_angle - current_angle - 360) < 45)) {
        if(target_angle > current_angle) {
            float returning_angle = current_angle + acceleration_rate*(current_time - last_angle_acceleration);
            current_angle = min(target_angle, returning_angle);
        } else {
            float returning_angle = current_angle - acceleration_rate*(current_time - last_angle_acceleration);
            current_angle = max(target_angle, returning_angle);
        }
    } else {
        if(current_speed < 10) {
            current_angle = target_angle;
        }
    }
    last_angle_acceleration = micros();
    return int(current_angle);
}


/*! @brief Sets the speed for each motor to move the robot in a certain direction at a certain speed.*/
void Motors::moveMotors(double angle, double rotation, int speed) {
    
    double motorSpeeds[motorno] = {rotation, rotation, rotation, rotation};
    if(speed != 0) {
        for(int i = 0; i < motorno; i++) {
            motorSpeeds[i] = cos(degreesToRandians(motorAngles4[i] + 90 - angle));
        }
        float highestValue = max(max(max(abs(motorSpeeds[0]), abs(motorSpeeds[1])), abs(motorSpeeds[2])), abs(motorSpeeds[3]));
        for (int i = 0; i < motorno; i++){
            motorSpeeds[i] = (motorSpeeds[i]/highestValue)*speed + rotation;
        }
        highestValue = max(max(max(abs(motorSpeeds[0]), abs(motorSpeeds[1])), abs(motorSpeeds[2])), abs(motorSpeeds[3]));
        for (int i = 0; i < motorno; i++){
            motorSpeeds[i] *= (speed / highestValue);
        }
    }
    motorA.moveMotor(motorSpeeds[0]);
    motorB.moveMotor(motorSpeeds[1]);
    motorC.moveMotor(motorSpeeds[2]);
    motorD.moveMotor(motorSpeeds[3]);
    
}