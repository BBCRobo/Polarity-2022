//Actual Github
#include <MotorController.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Pins.h>
#include <Common.h>
#include <Motors.h>
#include <LightSensors2.h>
#include <LightSensors3.h>
#include <LightSensors4.h>
#include <PID.h>
#include <Camera.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <imumaths.h>
#include <Attack.h>
#include <Defend.h>

Motors motorArray = Motors();
LightSensor2 ls;
LightSensor3 lsv;
LightSensor4 lsc;
Camera eyes;
MoveData move;
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
PID attackhorizontalPID(HEADING_AHP, HEADING_AHI, HEADING_AHD, 20);
PID attackverticalPID(HEADING_AVP, HEADING_AVI, HEADING_AVD, 20);
bno::Adafruit_BNO055 compass;
sensors_event_t gyro;
Attack attack;
Defend defend;
#define FindLS 0


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    // ls.init();
    lsc.init();
    eyes.init();
    //BNO Init
    while (!compass.begin()) {
        Serial.println("Seems that the BNO is dead....");
    }
    compass.setExtCrystalUse(true);
    move.surge = false;
    move.angle = -1;
    move.nosee = 0;
    if(FindLS == 1) {
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
            lsc.green[i] = (lsc.lsdebug[i][0] + lsc.lsdebug[i][1])/2;
            // Serial.print(lsv.green[i]);
            // Serial.print(" ");
            EEPROM.update(i, int(lsc.green[i]/4));
        }
        // Serial.println(";");
    }
    Serial.println("done");
}


void loop() {
    compass.getEvent(&gyro);
    eyes.read();
    move.straight = eyes.straight;

    //Position is determined by a 6x8 grid on the field.
    int position = -1;
    if(false) {
        float a_x = -1000;
        float a_y = -1000;
        float d_x = -1000;
        float d_y = -1000;
        if(eyes.aalive) {
            a_x = sin(floatMod(eyes.attackangle + gyro.orientation.x, 360)*M_PI/180)*eyes.attackdistance;
            a_y = cos(floatMod(eyes.attackangle + gyro.orientation.x, 360)*M_PI/180)*eyes.attackdistance;
        }
        if(eyes.dalive) {
            d_x = sin(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
            d_y = cos(floatMod(eyes.defendangle + gyro.orientation.x, 360)*M_PI/180)*eyes.defenddistance;
        }
        float pos_x = (a_x == -1000 ? 0 : a_x) + (d_x == -1000 ? 0 : d_x);
        float pos_y = (a_y == -1000 ? 0 : a_y) + (d_y == -1000 ? 0 : d_y);
    }

    if (true){
        // if(eyes.aalive && eyes.attackdistance < 100) {
        //     if(eyes.attackangle < 80) {
        //         move.correction = headingPID.update(gyro.orientation.x, gyro.orientation.x + eyes.attackangle);
        //     } else {
        //         move.correction = headingPID.update(gyro.orientation.x, gyro.orientation.x - 360 + eyes.attackangle);
        //     }
        // } else {
        move.correction = headingPID.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
        // }
        float horizontal_move = -attackhorizontalPID.update(sin(M_PI*eyes.straight/180)*eyes.balldist, abs(eyes.straight > 180 ? eyes.straight - 360 : eyes.straight) > 90 ? (eyes.straight > 180 ? (eyes.straight - 360)/10 : (eyes.straight)/10) : 0);
        float vertical_move = -attackverticalPID.update(cos(M_PI*eyes.straight/180)*eyes.balldist, 10);//abs(eyes.straight > 180 ? eyes.straight - 360 : eyes.straight) > 90 ? 0 : 10);
        move.angle = radiansToDegrees(atan2(horizontal_move, vertical_move));
        if(move.angle < 0) {
            move.angle += 360;
        }
        move.power = sqrt(vertical_move*vertical_move + horizontal_move*horizontal_move);
        // move.angle = attack.angle(eyes.aalive, eyes.balive, eyes.dalive, eyes.attackdistance, eyes.defendangle, eyes.balldist, move.straight);
        // move.power = pow(euler, 0.05*eyes.balldist)+15;
        if(abs(move.straight > 180 ? move.straight - 360 : move.straight) < 20 && eyes.balive) {
            move.angle = 0;
            move.power = 30;
        }
        if(eyes.balive != true) {move.angle = -1;}
        // Serial.print(eyes.straight);
        // Serial.print(" ");
        // Serial.print(horizontal_move);
        // Serial.print(" ");
        // Serial.print(vertical_move);
        // Serial.print(" ");
        // Serial.print(move.angle);
        // Serial.print(" ");
        // Serial.println(move.power);
    } else if(false){
        if(eyes.dalive) {
            move.correction = headingPID.update(gyro.orientation.x + 180, gyro.orientation.x + eyes.defendangle);
        } else {
            move.correction = headingPID.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0);
        }
        // move.correction = defend.correction(headingPID.update(gyro.orientation.x > 180 ? gyro.orientation.x - 360 : gyro.orientation.x, 0), eyes.dalive, eyes.defendangle);
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist);
    }
    // move.line = ls.DirectionOfLine(gyro.orientation.x);
    // move.line = lsv.DirectionOfLine(gyro.orientation.x, position);
    move.line = lsc.DirectionOfLine(gyro.orientation.x, position);
    // move.line = -11.25;
    move.angle = -1;
    if(move.line != -11.25) {
        move.angle = move.line;
        move.power = 20;
    }
    if(move.angle != -1) {
        motorArray.moveMotors(move.angle, move.correction, move.power);
    } else {
        motorArray.moveMotors(0, move.correction, 0);
    }
}