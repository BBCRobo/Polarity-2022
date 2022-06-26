#include <LightSensors3.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Common.h>

int LightSensor3::read(int no) {
    int multiplexerpins[4] = {LS_BINARY_0, LS_BINARY_1, LS_BINARY_2, LS_BINARY_3};
    if(no >= 16) {
        no -= 16;
        int power = 8;
        for(int i = 3; i >= 0; i--) {
            if(no >= power) {
                digitalWrite(multiplexerpins[i], HIGH);
                no -= power;
            } else {
                digitalWrite(multiplexerpins[i], LOW);
            }
            power = int(power/2);
        }
        return analogRead(LS_OUTPUT_2);
    } else {
        int power = 8;
        for(int i = 3; i >= 0; i--) {
            if(no >= power) {
                digitalWrite(multiplexerpins[i], HIGH);
                no -= power;
            } else {
                digitalWrite(multiplexerpins[i], LOW);
            }
            power = int(power/2);
        }
        return analogRead(LS_OUTPUT_1);
    }
}

void LightSensor3::init() {
    pinMode(LS_BINARY_0, OUTPUT);
    pinMode(LS_BINARY_1, OUTPUT);
    pinMode(LS_BINARY_2, OUTPUT);
    pinMode(LS_BINARY_3, OUTPUT);
    pinMode(LS_OUTPUT_1, INPUT);
    pinMode(LS_OUTPUT_2, INPUT);
    for(int i = 0; i < LS_COUNT; i++) {
        // green[i] = read(lspins[i]);
        green[i] = EEPROM.read(i)*4;
    }
    location = 0;
    prev = -11.25;
}

void LightSensor3::FindGreen() {
    unsigned long start_time = millis();
    for(int i = 0; i < LS_COUNT; i++) {
        lsdebug[i][0] = read(lspins[i]);
    }
    unsigned long end_time = millis();
    while(end_time-start_time < 5000) {
        for(int i = 0; i < LS_COUNT; i++) {
            int value = read(lspins[i]);
            if(value > lsdebug[i][0]) {
                lsdebug[i][0] = value;
            }
        }
        end_time = millis();
    }
}

void LightSensor3::FindWhite() {
    unsigned long start_time = millis();
    for(int i = 0; i < LS_COUNT; i++) {
        lsdebug[i][1] = read(lspins[i]);
    }
    unsigned long end_time = millis();
    while(end_time-start_time < 5000) {
        for(int i = 0; i < LS_COUNT; i++) {
            int value = read(lspins[i]);
            if(value > lsdebug[i][1]) {
                lsdebug[i][1] = value;
            }
        }
        end_time = millis();
    }
}

double LightSensor3::DirectionOfLine(float orientation, int position) {
    int lsvalues[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int totalno = 0;
    float total[2] = {0,0};
    double vector_direction[2] = {0,0};
    double expected[3] = {0,0,0};
    for(int i = 0; i < (LS_COUNT); i++) {
        Serial.print(read(lspins[i]));
        Serial.print(" ");
        if(int(read(lspins[i])/4)*4 > green[i]) {
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
            total[0] += vector_directions[i][0];
            total[1] += vector_directions[i][1];
        }
    }
    // Serial.println(';');
    vector_direction[0] = total[0]/totalno;
    vector_direction[1] = total[1]/totalno;
    vector_modulus = sqrt(pow(vector_direction[0], 2) + pow(vector_direction[1], 2));
    direction = atan2(vector_direction[1],vector_direction[0]);
    // Serial.print(direction);
    // Serial.print(" ");
    direction = (orientation > 180 ? orientation - 360 : orientation)*M_PI/180 + direction;
    while(direction < -M_PI) {
        direction += M_PI*2;
    }
    while(direction >= M_PI) {
        direction -= M_PI*2;
    }
    // Serial.println(direction);
    // Serial.print(vector_direction[0]);
    // Serial.print(" ");
    // Serial.print(vector_direction[1]);
    // Serial.print(" ");
    // Serial.print(vector_modulus);
    // Serial.print(" ");
    // Serial.print(direction);
    // Serial.print(" ");
    // if(position != -1) {
    //     if(position%10 < 2) {
    //         if(position-position%10 < 2) {
    //             expected[0] = M_PI/2;
    //             expected[1] = M_PI;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI;
    //             expected[1] = -M_PI/2;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = -M_PI/2;
    //             expected[1] = M_PI/2;
    //             expected[2] = 1;
    //         }
    //     } else if(position%10 > 5) {
    //         if(position-position%10 < 2) {
    //             expected[0] = 0;
    //             expected[1] = M_PI/2;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI/2;
    //             expected[1] = 0;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = -M_PI/2;
    //             expected[1] = M_PI/2;
    //             expected[2] = 0;
    //         }
    //     } else {
    //         if(position-position%10 < 2) {
    //             expected[0] = 0;
    //             expected[1] = M_PI;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI;
    //             expected[1] = 0;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = 0;
    //             expected[1] = 0;
    //             expected[2] = -1;
    //         }
    //     }
    // } else {
    //     expected[0] = 0;
    //     expected[1] = 0;
    //     expected[2] = -2;
    // }

    if(totalno != 0) {
        if(location == 0) {
            location = 1;
            prev = direction;
        }
        if(location == 3) {
            location = 2;
        }
        if(vector_modulus > 0.75 && (abs(prev - direction) < 0.5 || abs(prev - direction + M_PI*2) < 0.5 || abs(prev - direction - M_PI*2) < 0.5)) {
            if(location == 2) {
                location = 1;
            }
        } else if(abs(prev - direction) > 0.5 || vector_modulus < 0.5) {
            if(location == 1) {
                location = 2;
            }
        }
        // if(expected[2] == 0) {
        //     if(location == 1) {
        //         if(direction > expected[1] || direction < expected[0]) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else if(vector_modulus < 0.5) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else {
        //             prev = direction;
        //         }
        //     } else if(location == 2) {
        //         if(direction < expected[1] && direction > expected[0] && vector_modulus >= 0.5) {
        //             location = 1;
        //             prev = direction;
        //         } else {
        //             direction = prev;
        //         }
        //     }
        // } else if(expected[2] == 1) {
        //     if(location == 1) {
        //         if(direction < expected[1] && direction > expected[0]) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else if(vector_modulus < 0.5) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else {
        //             prev = direction;
        //         }
        //     } else if(location == 2) {
        //         if((direction > expected[1] || direction < expected[0]) && vector_modulus >= 0.5) {
        //             location = 1;
        //             prev = direction;
        //         } else {
        //             direction = prev;
        //         }
        //     }
        // } else if(expected[2] < 0) {
        if(location == 1) {
            prev = direction;
        } else if(location == 2) {
            direction = prev;
        }
        // }
    } else {
        if(location == 1 || location == 0) {
            location = 0;
            direction = -11.25;
            prev = -11.25;
        } else if(location == 2 || location == 3) {
            // if(expected[2] == -1) {
            //     location = 0;
            //     direction = -11.25;
            //     prev = -11.25;
            // } else {
            location = 3;
            direction = prev;
            // }
        }
    }
    if(direction != -11.25) {
        direction -= (orientation > 180 ? orientation - 360 : orientation)*M_PI/180;
        while(direction < 0) {
            direction += M_PI*2;
        }
        while(direction >= M_PI*2) {
            direction -= M_PI*2;
        }
        direction *= 180/M_PI;
    }
    // Serial.println(direction);
    return direction;
}