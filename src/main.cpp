//Actual Github
#include <MotorController.h>
#include <Arduino.h>
#include <Pins.h>
#include <Common.h>
#include <Motors.h>
#include <LightSensors2.h>
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
Camera eyes;
MoveData move;
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
// PID attackPID(HEADING_AP, HEADING_AI, HEADING_AD, HEADING_MAX_CORRECTION);
bno::Adafruit_BNO055 compass;
sensors_event_t eventa;
Attack attack;
Defend defend;


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    ls.init();
    eyes.init();
    //BNO Init
    while (!compass.begin()) {
        Serial.println("Seems that the BNO is dead....");
    }
    compass.setExtCrystalUse(true);
    move.surge = false;
    move.angle = -1;
    move.nosee = 0;
    Serial.println("done");
}


void loop() {
    compass.getEvent(&eventa);
    eyes.read();
    move.straight = eyes.straight;

    move.power = 20;
    if (true){
        move.correction = attack.correction(headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0), eyes.attackangle, eyes.aalive, eyes.balldist);
        move.angle = attack.angle(eyes.aalive, eyes.balive, eyes.dalive, eyes.attackdistance, eyes.defendangle, eyes.balldist, move.straight);
        if(move.angle == 0 && eyes.balive) {move.power = 30;}
    } else if(false){
        move.correction = defend.correction(headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0), eyes.dalive, eyes.defendangle);
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist);
    }
    move.line = ls.DirectionOfLine(eventa.orientation.x);
    // move.line = -11;
    move.angle = -1;
    if(move.line != -11) {
        move.angle = move.line;
        move.power = 30;
    }
    if(move.angle != -1) {
        motorArray.moveMotors(move.angle, move.correction, move.power);
    } else {
        motorArray.moveMotors(0, move.correction, 0);
    }
}