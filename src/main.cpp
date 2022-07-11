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
#include <math.h>


Motors motorArray = Motors();
LightSensor4 lsc;
Camera eyes;
Position position;
Bluetooth teeth;
MoveData move;
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
bno::Adafruit_BNO055 compass;
sensors_event_t gyro;
Attack attack;
Defend defend;
int loop_count;


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    lsc.init();
    eyes.init();
    position.init();
    //BNO Init
    while (!compass.begin()) {
        Serial.println("Seems that the BNO is dead....");
    }
    compass.setExtCrystalUse(true);
    move.angle = -1;
    if(FindLS) {
        lsc.FindGreen();
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        lsc.FindWhite();
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        for(int i = 0; i < LS_COUNT; i++) {
            lsc.green[i] = lsc.lsdebug[i][0]*0.5 + lsc.lsdebug[i][1]*0.5;
            if(i == 0 && ROBOT == 2) {
                lsc.green[i] = lsc.lsdebug[i][0]*0.6 + lsc.lsdebug[i][1]*0.4;
                lsc.green[i] -= 60;
            }
            // Serial.print(lsv.green[i]);
            // Serial.print(" ");
            EEPROM.update(i, int(lsc.green[i]/4));
        }
        // Serial.println(";");
    }
    // pinMode(KICKER_PIN, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    // digitalWrite(KICKER_PIN, LOW);
    move.attacker = ATTACKER;
    move.defender = DEFENDER;
    loop_count = 0;
    Serial.println("done");
}


void loop() {
    compass.getEvent(&gyro);
    eyes.read();
    move.straight = eyes.straight;
    position.read_lrfs();
    position.update(gyro.orientation.x, eyes.attackangle, eyes.attackdistance, eyes.defendangle, eyes.defenddistance, eyes.straight, eyes.balldist, eyes.aalive, eyes.dalive);
    // if(eyes.dalive || eyes.aalive || eyes.balive) {
    //     Serial.println("y");
    //     teeth.update(0, 0, 0, 0);
    // } else {
    //     teeth.update(255,255,255,255);
    // }
    // Serial.println(teeth.other_ball_location[0]);

    //Position is determined by a 6x8 grid on the field.
    int position = -1;
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
    // position = 10*int(r_l/270)+int(b_l/270);
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
    // Serial.println(position);

    if (move.attacker){
        if(eyes.aalive && (abs(eyes.attackangle - 0) < 40 || abs(eyes.attackangle - 360) < 40) && eyes.balldist < 20) {
            if(eyes.attackangle < 180) {
                move.correction = headingPID.update(gyro.orientation.x, gyro.orientation.x + eyes.attackangle);
            } else {
                move.correction = headingPID.update(gyro.orientation.x, gyro.orientation.x - 360 + eyes.attackangle);
            }
        } else {
            move.correction = headingPID.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
        }
        move.angle = attack.angle(eyes.balive, eyes.balldist, eyes.straight);
        move.power = attack.power(eyes.straight, eyes.balldist);
    } else if(move.defender) {
        if(eyes.dalive) {
            move.correction = headingPID.update(gyro.orientation.x + 180, gyro.orientation.x + eyes.defendangle);
        } else {
            move.correction = headingPID.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
        }
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive, gyro.orientation.x);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist, gyro.orientation.x);
    }
    move.line = lsc.DirectionOfLine(gyro.orientation.x, position);
    move.line = -11.25;
    // move.angle = -1;
    if(move.line != -11.25) {
        move.angle = move.line;
        move.power = 20;
    }
    if(move.angle != -1) {
        motorArray.moveMotors(move.angle, move.correction, move.power);
    } else {
        motorArray.moveMotors(0, move.correction, 0);
    }
    loop_count += 1;
}