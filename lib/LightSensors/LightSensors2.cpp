#include <LightSensors2.h>
#include <Arduino.h>
#include <Common.h>

//Hello M8

int LightSensor2::read(int no) {
    int multiplexerpins[4] = {LS_BINARY_0, LS_BINARY_1, LS_BINARY_2, LS_BINARY_3};
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
    return analogRead(LS_OUTPUT);
}

void LightSensor2::init() {
    pinMode(LS_BINARY_0, OUTPUT);
    pinMode(LS_BINARY_1, OUTPUT);
    pinMode(LS_BINARY_2, OUTPUT);
    pinMode(LS_BINARY_3, OUTPUT);
    pinMode(LS_OUTPUT, INPUT);
    for(int i = 0; i < LS_COUNT; i++) {
        green[i] = read(lspins[i]);
    }
    prev = -11.25;
    location = 0;
    orig = -11.25;
    wrong = false;
}

double LightSensor2::DirectionOfLine() {
    int lsvalues[LS_COUNT] = {0};
    int totalno = 0;
    int total = 0;
    for(int i = 0; i < LS_COUNT; i++) {
        // Serial.print(read(lspins[i]));
        // Serial.print(" ");
        //Serial.print(i);
        // Serial.print(", ");
        if(read(lspins[i]) > green[i] + 200) {
            lsvalues[i] += 1;
            totalno += 1;
        }
    }
    // Serial.print(read(lspins[0]));
    // Serial.println(";");
    for(int i = 0; i < LS_COUNT; i++) {
        // Serial.print(lsvalues[i]);
        // Serial.print(" ");
        if(lsvalues[i] > 0) {
            if((lsvalues[8] > 0 || lsvalues[9] > 0 || lsvalues[10] > 0 || lsvalues[11] > 0) && (lsvalues[7] > 0 || lsvalues[6] > 0 || lsvalues[5] > 0)) {
                total += direc2[i];
                if((lsvalues[0] > 0 || lsvalues[1] > 0) && (lsvalues[15] > 0 || lsvalues[14] > 0)) {
                    wrong = true;
                }
            } else {
                total += direc[i];
            }
        }
    }
    direction = total / (float)totalno;
    if(direction > 359) {
        direction -= 360;
    }
    // Serial.print(prev);
    // Serial.print(" ");
    // Serial.print(direction);
    // Serial.print(" ");
    // Serial.println(location);
    // Serial.println(";");
    if(totalno != int(0)) {
        if(location == 0) {
            location = 1;
            prev = direction;
            orig = direction;
        } else if(location == 3) {
            location = 2;
            direction = prev;
        } else if(abs(prev - direction) < 70 || abs(prev + 360 - direction) < 70 || abs(prev - direction - 360) < 70) {
            if(location == 2) {
                location = 1;
                direction = prev;
            }
        } else if(location == 2) {
            direction = prev;
        } else if(abs(prev - direction) > 70) {
            if(location == 1) {
                location = 2;
                direction = prev;
            }
        } if(location == 1) {
            if(wrong == true) {
                location = 2;
                prev = orig;
                direction = orig;
            } else {
                if(abs(225-direction) < 35) {
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
        }
    } else if(totalno == int(0)) {
        if(location == 1 || location == 0) {
            location = 0;
            direction = -11.25;
            prev = -11.25;
            orig = -11.25;
        } else if(location == 2 || location == 3) {
            location = 3;
            direction = prev;
        }
    }
    wrong = false;
    return direction;
};