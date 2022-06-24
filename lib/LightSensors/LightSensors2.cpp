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
}

double LightSensor2::DirectionOfLine(float orientation) {
    int lsvalues[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int totalno = 0;
    int total = 0;
    wrong = false;
    for(int i = 0; i < LS_COUNT; i++) {
        Serial.print(read(lspins[i]));
        Serial.print(" ");
        if(read(lspins[i]) > green[i] + 80) {
            lsvalues[i] = 1;
            totalno += 1;
        } else {
            lsvalues[i] = 0;
        }
    }
    Serial.println(";");
    for(int i = 0; i < LS_COUNT; i++) {
        // Serial.print(lsvalues[i]);
        // Serial.print(' ');
        if(lsvalues[i] > 0) {
            if((lsvalues[16] > 0 || lsvalues[17] > 0 || lsvalues[18] > 0 || lsvalues[19] > 0) && (lsvalues[15] > 0 || lsvalues[14] > 0 || lsvalues[13] > 0)) {
                if((lsvalues[0] > 0 || lsvalues[1] > 0 || lsvalues[2] || lsvalues[3]) && (lsvalues[31] > 0 || lsvalues[30] > 0 || lsvalues[29] || lsvalues[28])) {
                    wrong = true;
                } else {
                    total += direc2[i];
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
    // Serial.print(prev);
    // Serial.print(" ");
    direction -= offby;
    // Serial.print(direction);
    // Serial.print(" ");
    if(totalno != int(0)) {
        if(location == 0) {
            location = 1;
            orig = direction;
            prev = direction;
        }
        if(location == 3) {
            location = 2;
        }
        if(abs(prev - direction) < 70 || abs(prev + 360 - direction) < 70 || abs(prev - direction - 360) < 70) {
            if(location == 2) {
                location = 1;
            }
        } else if(abs(prev - direction) > 70) {
            if(location == 1) {
                location = 2;
            }
        }
        if(location == 1) {
            if(wrong) {
                location = 2;
            } else if(abs(225-direction) < 35) {
                prev = 225;
            } else if(abs(315 - direction) < 26) {
                prev = 315;
            } else if(abs(45 - direction) < 26) {
                prev = 45;
            } else if(abs(135 - direction) < 26) {
                prev = 135;
            } else if(abs(90 - direction) < 26) {
                prev = 90;
            } else if(abs(180 - direction) < 26) {
                prev = 180;
            } else if(abs(270 - direction) < 26) {
                prev = 270;
            } else if(abs(0 - direction) < 26 || abs(0 - direction - 360) < 26 || abs(360 - direction) < 26) {
                prev = 0;
            }
        }
        if(location == 2) {
            direction = (orig+prev)/2;
        }
    } else if(totalno == int(0)) {
        if(location == 1 || location == 0) {
            location = 0;
            direction = -11.25;
            orig = -11.25;
            prev = -11.25;
            offby = 0;
        } else if(location == 2 || location == 3) {
            location = 3;
            direction = prev;
        }
    }
    // Serial.println(location);
    return (direction + offby);
};