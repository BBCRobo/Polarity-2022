#include <Motors.h>

Motors::Motors(){};

void Motors::init() {
    motorA.init(INONE_FR, INTWO_FR, PWM_FR);
    motorB.init(INONE_FL, INTWO_FL, PWM_FL);
    motorC.init(INONE_BR, INTWO_BR, PWM_BR);
    motorD.init(INONE_BL, INTWO_BL, PWM_BL);
}

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