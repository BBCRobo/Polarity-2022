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
#include <LRF.h>
#include <Bluetooth.h>


Motors motorArray = Motors();
LightSensor4 lsc;
Camera eyes;
LRF laser;
Bluetooth teeth;
MoveData move;
PID headingPID(HEADING_KP, HEADING_KI, HEADING_KD, HEADING_MAX_CORRECTION);
bno::Adafruit_BNO055 compass;
sensors_event_t gyro;
Attack attack;
Defend defend;


void setup() {
    Serial.begin(9600);
    Serial.print("Setup Started...");
    motorArray.init();
    lsc.init();
    eyes.init();
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
    // pinMode(13, OUTPUT);
    // digitalWrite(KICKER_PIN, LOW);
    move.attacker = ATTACKER;
    move.defender = DEFENDER;
    Serial.println("done");
}


void loop() {
    // delay(10000);
    // digitalWrite(KICKER_PIN, HIGH);
    // digitalWrite(13, HIGH);
    // delay(1000);
    // digitalWrite(13, LOW);
    // delay(10000);
    // digitalWrite(KICKER_PIN, LOW);
    // digitalWrite(13, HIGH);
    // delay(1000);
    // digitalWrite(13, LOW);
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

    if (move.attacker){
        if(eyes.aalive && abs(move.straight > 180 ? move.straight - 360 : move.straight) < 60 && eyes.balldist < 25) {
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
        Serial.println(eyes.balldist);
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
}