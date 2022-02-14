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
    pinMode(MOTOR_SWITCH, INPUT);
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
    double oldangle = move.angle;
    eyes.read();
    if(move.angle <= 0) {
        move.angle = oldangle;
        move.nosee += 1;
    } else {
        move.straight = eyes.straight;
        if(eyes.balive) {
            move.nosee = 0;
        }
    }
    if(move.nosee > 30) {
        move.angle = -1;
    }

    move.power = 18;
    if (true){
        move.correction = attack.correction(headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0), eyes.attackangle, eyes.aalive, eyes.balldist);
        move.angle = attack.angle(eyes.aalive, eyes.balive, eyes.dalive, eyes.attackdistance, eyes.defendangle, eyes.balldist, move.straight);
        if(move.angle == 0 && eyes.balive) {move.power = 24;}
    } else if(false){
        move.correction = defend.correction(headingPID.update(eventa.orientation.x > 180 ? eventa.orientation.x - 360 : eventa.orientation.x, 0), eyes.dalive);
        move.angle = defend.angle(move.straight, eyes.defenddistance, eyes.balldist, eyes.dalive, eyes.balive);
        move.power = defend.power(eyes.dalive, eyes.balive, move.straight, eyes.defenddistance, eyes.balldist);
    }

    move.line = ls.DirectionOfLine();
    if(move.line != -11) {
        move.angle = move.line;
        move.power = 30;
    }
    if(digitalRead(MOTOR_SWITCH) == 0) { move.angle = -5; }
    if(move.angle != -5) {
        if(move.angle != -1) {
            motorArray.moveMotors(move.angle, move.correction, move.power);
        } else {
            motorArray.moveMotors(0, move.correction, 0);
        }
    } else {
        motorArray.moveMotors(0, 0, 0);
    }
}