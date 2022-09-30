//Actual Github
#include <MotorController.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Pins.h>
#include <Common.h>
#include <Motors.h>
#include <LightSensors4.h>
#include <PID.h>
#include <Camera.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <imumaths.h>
#include <Attack.h>
#include <Defend.h>
#include <Bluetooth.h>
#include <Position.h>
#include <Kicker.h>
#include <math.h>


Motors motorArray = Motors();
LightSensor4 lsc;
Camera eyes;
Position pos;
Bluetooth teeth;
MoveData move;
PID headingPID1(HEADING_KP1, HEADING_KI1, HEADING_KD1, HEADING_MAX_CORRECTION);
PID headingPID2(HEADING_KP2, HEADING_KI2, HEADING_KD2, HEADING_MAX_CORRECTION);
bno::Adafruit_BNO055 compass;
sensors_event_t gyro;
Attack attack;
Defend defend;
unsigned long start_time;
bool spin;
bool start;
int loops;


void dance(float heading) {
    unsigned long current_time = millis();
    if(start) {
        if(current_time - start_time >= 11000) {
            start = false;
            start_time = millis();
        } else if(current_time - start_time > 7000 && current_time - start_time < 11000) {
            motorArray.moveMotors(0, headingPID2.update(heading, 180), 10);
        } else {
            motorArray.moveMotors(0, headingPID2.update(heading, 180), 0);
        }
    } else {
        if(spin) {
            if(current_time - start_time < 923) {
                motorArray.moveMotors(0, 10*pow(-1, loops%2), 0);
            } else {
                spin = false;
                start_time = millis();
                loops += 1;
            }
        } else {
            if(current_time - start_time < 923*1.5) {
                motorArray.moveMotors(270, headingPID2.update(heading, 180), 10);
            } else if(current_time - start_time < 923*3) {
                motorArray.moveMotors(90, headingPID2.update(heading, 180), 10);
            } else {
                spin = true;
                start_time = millis();
            }
        }
    }
}


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    lsc.init();
    eyes.init();
    // defend.kick_init();
    // pos.init();
    teeth.init();
    //BNO Init
    while (!compass.begin()) {
        Serial.println("Seems that the BNO is dead....");
    }
    compass.setExtCrystalUse(true);
    move.angle = -1;
    if(FindLS) {
        lsc.FindGreen();
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        lsc.FindWhite();
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        for(int i = 0; i < LS_COUNT*2; i++) {
            lsc.green[i/2] = lsc.lsdebug[i/2][0]*0.5 + lsc.lsdebug[i/2][1]*0.5;
            uint16_t grn = lsc.green[i/2];
            EEPROM.update(i, (uint8_t)((grn >> 8) & 0xFF));
            i++;
            EEPROM.update(i, (uint8_t)(grn & 0xFF));
            // EEPROM.update(i, int(lsc.green[i]/4));
        }
    }
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    move.attacker = ATTACKER;
    move.defender = DEFENDER;
    delay(1000);
    Serial.println("done");
    delay(3000);
    start_time = millis();
    start = true;
    spin = false;
    loops = 0;
}


void loop() {
    compass.getEvent(&gyro);
    eyes.read(gyro.orientation.x);
    move.straight = eyes.straight;
    // pos.read_lrfs();
    pos.update(gyro.orientation.x, eyes.attackangle, eyes.attackdistance, eyes.defendangle, eyes.defenddistance, eyes.straight, eyes.balldist, eyes.aalive, eyes.dalive);
    // Serial.print(eyes.aalive);
    // Serial.print(" ");
    // Serial.print(eyes.dalive);
    // Serial.print(" ");
    // Serial.print(eyes.balive);
    // Serial.print(" ");
    // Serial.print(pos.pos_x);
    // Serial.print(" ");
    // Serial.print(pos.pos_y);
    // Serial.print(" ");
    // Serial.print(pos.ball_pos_x);
    // Serial.print(" ");
    // Serial.print(pos.ball_pos_y);
    // Serial.print(" ");
    // Serial.println(pos.position);

    if(eyes.balive) {
        teeth.update(int(pos.pos_x), int(pos.pos_y), int(pos.ball_pos_x), int(pos.ball_pos_y));
    } else {
        teeth.update(int(pos.pos_x), int(pos.pos_y), 255, 0);
        if(teeth.other_ball_location[0] != -1) {
            eyes.straight = floatMod(atan2(teeth.other_ball_location[0], teeth.other_ball_location[1]), 360);
            eyes.balldist = sqrt(pow(teeth.other_ball_location[0]-pos.pos_x,2) + pow(teeth.other_ball_location[1]-pos.pos_y,2));
        }
    }
    if(teeth.connection == false) {
        move.attacker = false;
        move.defender = true;
    } else {
        // Add Surging!!!
        if(teeth.other_robot_location[1] != -1 && teeth.other_robot_location[1] < pos.pos_y) {
            move.attacker = false;
            move.defender = true;
        } else if(teeth.other_robot_location[1] != -1 && teeth.other_robot_location[1] > pos.pos_y) {
            move.attacker = true;
            move.defender = false;
        }
    }

    //Position is determined by a 6x8 grid on the field. Old Code
    // float a_x = -1000;
    // float a_y = -1000;
    // float d_x = -1000;
    // float d_y = -1000;
    // float b_l = -1000;
    // float f_l = -1000;
    // float l_l = -1000;
    // float r_l = -1000;
    // float pos_x = -1000;
    // float pos_y = -1000;
    // if(laser.lrf_init[1]) {
    //     b_l = 2050 + laser.lrf_values[1]*cos(floatMod(180 + gyro.orientation.x, 360)*M_PI/180);
    // }
    // if(laser.lrf_init[0]) {
    //     f_l = laser.lrf_values[0]*cos(floatMod(gyro.orientation.x, 360)*M_PI/180);
    // }
    // if(laser.lrf_init[2]) {
    //     l_l = -laser.lrf_values[2]*sin(floatMod(270 + gyro.orientation.x, 360)*M_PI/180);
    // }
    // if(laser.lrf_init[3]) {
    //     r_l = 1570 - laser.lrf_values[3]*sin(floatMod(90 + gyro.orientation.x, 360)*M_PI/180);
    // }
    // if(true) {
    //     if(eyes.aalive) {
    //         a_x = 785-10*sin(floatMod(eyes.attackangle + gyro.orientation.x, 360)*M_PI/180)*eyes.attackdistance;
    //         a_y = 270+10*cos(floatMod(eyes.attackangle + gyro.orientation.x, 360)*M_PI/180)*eyes.attackdistance;
    //         if(eyes.dalive) {
    //             d_x = 785-10*sin(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
    //             d_y = 1780+10*cos(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
    //             pos_x = 0.5*a_x + 0.5*d_x;
    //             pos_y = 0.5*a_y + 0.5*d_y;
    //         } else {
    //             pos_x = a_x;
    //             pos_y = a_y;
    //         }
    //     } else {
    //         if(eyes.dalive) {
    //             d_x = 785-10*sin(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
    //             d_y = 1780+10*cos(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
    //             pos_x = d_x;
    //             pos_y = d_y;
    //         }
    //     }
    // }
    // pos = 10*int(r_l/270)+int(b_l/270);
    // Serial.print(f_l);
    // Serial.print(" ");
    // Serial.print(b_l);
    // Serial.print(" ");
    // Serial.print(l_l);
    // Serial.print(" ");
    // Serial.print(r_l);
    // Serial.print(" ");
    // Serial.print(a_x);
    // Serial.print(" ");
    // Serial.print(a_y);
    // Serial.print(" ");
    // Serial.print(d_x);
    // Serial.print(" ");
    // Serial.print(d_y);
    // Serial.print(" ");
    // Serial.println(pos);
    // if(!eyes.dalive && eyes.balive) {
    //     move.attacker = true;
    //     move.defender = false;
    // } else {
    //     move.defender = true;
    //     move.attacker = false;
    // }

    if (move.attacker){
        //This section of the code was originally a separate function until the goal tracking PID got removed. It keeps the robot facing the correct direction.
        if(eyes.aalive && eyes.balldist < 30) {
            // if((eyes.attackangle < 30 || eyes.attackangle > 330) && eyes.balldist < 15) {
            //     kick.fire();
            // }
            if(eyes.attackangle < 180) {
                if(ROBOT == 1) {
                    move.correction = headingPID1.update(gyro.orientation.x, gyro.orientation.x + eyes.attackangle);
                } else {
                    move.correction = headingPID2.update(gyro.orientation.x, gyro.orientation.x + eyes.attackangle);
                }
            } else {
                if(ROBOT == 1) {
                    move.correction = headingPID1.update(gyro.orientation.x, gyro.orientation.x - 360 + eyes.attackangle);
                } else {
                    move.correction = headingPID2.update(gyro.orientation.x, gyro.orientation.x - 360 + eyes.attackangle);
                }
            }
        } else {
            if(ROBOT == 1) {
                move.correction = headingPID1.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
            } else {
                move.correction = headingPID2.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
            }
        }
        move.angle = attack.angle(eyes.balive, eyes.balldist, eyes.straight, pos.pos_x, pos.pos_y, eyes.attackdistance);
        move.power = attack.power(eyes.straight, eyes.balldist, eyes.balive);
    } else if(move.defender) {
        //This section of the code was originally a separate function until the goal tracking PID got removed. It keeps the robot facing the correct direction.
        if(eyes.dalive && !defend.check_kick()) {
            if(ROBOT == 1) {
                move.correction = headingPID1.update(gyro.orientation.x + 180, gyro.orientation.x + eyes.defendangle);
            } else {
                move.correction = headingPID2.update(gyro.orientation.x + 180, gyro.orientation.x + eyes.defendangle);
            }
        } else {
            if(ROBOT == 1) {
                move.correction = headingPID1.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
            } else {
                move.correction = headingPID2.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
            }
        }
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive, gyro.orientation.x, eyes.attackdistance);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist, gyro.orientation.x);
        // if(eyes.balldist <= CLOSE_BALL && eyes.defenddistance < CENTER_Y && abs(move.straight > 180 ? move.straight - 360 : move.straight) < 20) {
        //     kick.fire();
        // }
    }
    move.line = lsc.DirectionOfLine(gyro.orientation.x, pos.position);
    // if(move.defender && eyes.dalive && eyes.defenddistance < 40) {
    //     move.line = -11.25;
    // }
    // move.line = -11.25;
    // move.angle = -1;
    if(move.line != -11.25) {
        move.angle = move.line;
        move.power = 20;
    }
    // move.angle = motorArray.slow_angle_change(move.angle);
    // move.power = motorArray.slow_speed_change(move.power, move.angle);
    // dance(gyro.orientation.x);
    if(move.angle != -1) {
        motorArray.moveMotors(move.angle, move.correction, move.power);
    } else {
        motorArray.moveMotors(0, move.correction, 0);
    }
}