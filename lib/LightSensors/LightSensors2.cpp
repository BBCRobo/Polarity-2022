#include <LightSensors2.h>
#include <Arduino.h>
#include <Common.h>

int LightSensor2::read(int no) {
    int multiplexerpins[4] = {LS_BINARY_0, LS_BINARY_1, LS_BINARY_2, LS_BINARY_3};
    if(no >= 16) {
        no -= 16;
        int power = 8;
        for(int i = 3; i >= 0; i--) {
            if(no >= power) {
                digitalWrite(multiplexerpins[i], true);
                no -= power;
            } else {
                digitalWrite(multiplexerpins[i], false);
            }
            power = int(power/2);
        }
        return analogRead(LS_OUTPUT_2);
    }
    int power = 8;
    for(int i = 3; i >= 0; i--) {
        if(no >= power) {
            digitalWrite(multiplexerpins[i], true);
            no -= power;
        } else {
            digitalWrite(multiplexerpins[i], false);
        }
        power = int(power/2);
    }
    return analogRead(LS_OUTPUT_1);
}

void LightSensor2::init() {
    pinMode(LS_BINARY_0, OUTPUT);
    pinMode(LS_BINARY_1, OUTPUT);
    pinMode(LS_BINARY_2, OUTPUT);
    pinMode(LS_BINARY_3, OUTPUT);
    pinMode(LS_OUTPUT_1, INPUT);
    pinMode(LS_OUTPUT_2, INPUT);
    for(int i = 0; i < LS_COUNT; i++) {
        green[i] = read(lspins[i]);
    }
    location = 0;
    orig = -11.25;
    wrong = false;
}

double LightSensor2::DirectionOfLine(float orientation) {
    int lsvalues[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int totalno = 0;
    int total = 0;
    for(int i = 0; i < LS_COUNT; i++) {
        // Serial.print(read(lspins[i]));
        // Serial.print(" ");
        if(read(lspins[i]) > green[i] + 100) {
            lsvalues[i] = 1;
            totalno += 1;
        } else {
            lsvalues[i] = 0;
        }
    }
    // Serial.println(";");
    for(int i = 0; i < LS_COUNT; i++) {
        // Serial.print(lsvalues[i]);
        // Serial.print(' ');
        if(lsvalues[i] > 0) {
            if((lsvalues[16] > 0 || lsvalues[17] > 0 || lsvalues[18] > 0 || lsvalues[19] > 0) && (lsvalues[15] > 0 || lsvalues[14] > 0 || lsvalues[13] > 0)) {
                total += direc2[i];
                if((lsvalues[0] > 0 || lsvalues[1] > 0) && (lsvalues[31] > 0 || lsvalues[30] > 0)) {
                    wrong = true;
                }
            } else {
                total += direc[i];
            }
        }
    }
    // Serial.println(';');
    direction = total / (float)totalno;
    if(direction > 359) {
        direction -= 360;
    }
    if(location == int(0)) {
        origorientation = orientation;
        offby = 0;
    } else {
        if(origorientation <= 180) {
            if(orientation > (180+origorientation)) {
                offby = abs(origorientation - orientation + 360);
            } else if(orientation < origorientation) {
                offby = origorientation - orientation;
            } else if(orientation <= (origorientation + 180)) {
                offby = origorientation - orientation;
            }
        } else if(origorientation > 180) {
            if(orientation <(origorientation - 180)) {
                offby = origorientation - orientation - 360;
            } else if((orientation <= origorientation) && (orientation >= (origorientation - 180))) {
                offby = origorientation - orientation;
            } else if(orientation > origorientation) {
                offby = origorientation - orientation;
            }
        }
    }    
    // Serial.print(direction);
    // Serial.print(' ');
    // Serial.print(offby);
    // Serial.print(' ');
    if(totalno != int(0)) {
        if(location == 0) {
            location = 1;
            orig = direction;
        } else if(location == 3) {
            location = 2;
            direction = orig;
        } else if(abs(orig - direction + offby) < 90 || abs(orig + offby + 360 - direction) < 90) {
            if(location == 2) {
                location = 1;
                direction = orig;
            }
        } else if(location == 2) {
            direction = orig;
        } else if(abs(orig - direction + offby) > 90) {
            if(location == 1) {
                location = 2;
                direction = orig;
            }
        } if(location == 1) {
            if(wrong == true) {
                location = 2;
                direction = orig;
            }
        }
    } else if(totalno == int(0)) {
        if(location == 1 || location == 0) {
            location = 0;
            direction = -11.25;
            orig = -11.25;
        } else if(location == 2 || location == 3) {
            location = 3;
            direction = orig;
        }
    }
    wrong = false;
    // Serial.print(orig);
    // Serial.print(' ');
    // Serial.println(location);
    return (direction);
};