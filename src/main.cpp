//Actual Github
#include <MotorController.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Pins.h>
#include <Common.h>
#include <Motors.h>
#include <LightSensors2.h>
#include <LightSensors3.h>
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
Camera eyes;
MoveData move;
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
// PID attackPID(HEADING_AP, HEADING_AI, HEADING_AD, HEADING_MAX_CORRECTION);
bno::Adafruit_BNO055 compass;
sensors_event_t eventa;
Attack attack;
Defend defend;
#define FindLS 1


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    // ls.init();
    lsv.init();
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
        lsv.FindGreen();
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        lsv.FindWhite();
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        for(int i = 0; i < LS_COUNT; i++) {
            lsv.green[i] = (lsv.lsdebug[i][0] + lsv.lsdebug[i][1])/2;
            // EEPROM.update(i, int(lsv.green[i]/4));
        }
    }
    Serial.println("done");
}


void loop() {
    bool yellow_attack = true;
    compass.getEvent(&eventa);
    eyes.read();
    move.straight = eyes.straight;

    //Position is determined by a 6x8 grid on the field.
    int position = -1;

    move.power = 20;
    if (true){
        // if(eyes.aalive && eyes.attackdistance < 100) {
        //     if(eyes.attackangle < 180) {
        //         move.correction = headingPID.update(eventa.orientation.x, eventa.orientation.x + eyes.attackangle);
        //     } else {
        //         move.correction = headingPID.update(eventa.orientation.x, eventa.orientation.x - 360 + eyes.attackangle);
        //     }
        // } else {
        move.correction = headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0);
        // }
        move.angle = attack.angle(eyes.aalive, eyes.balive, eyes.dalive, eyes.attackdistance, eyes.defendangle, eyes.balldist, move.straight);
        if(move.angle == 0 && eyes.balive) {move.power = 30;}
    } else if(false){
        if(eyes.dalive) {
            move.correction = headingPID.update(eventa.orientation.x + 180, eventa.orientation.x + eyes.defendangle);
        } else {
            move.correction = headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0);
        }
        // move.correction = defend.correction(headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0), eyes.dalive, eyes.defendangle);
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist);
    }
    // move.line = ls.DirectionOfLine(eventa.orientation.x);
    move.line = lsv.DirectionOfLine(eventa.orientation.x, position);
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