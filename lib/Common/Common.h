#ifndef COMMON_H
#define COMMON_H

#include <math.h>

#define TO_RADIANS 0.01745329251994329576923690768489

double degreesToRandians(double degrees);

#define TO_DEGREES 57.295779513082320876798154814105

#define euler 2.7182818284590452353602874713527

double radiansToDegrees(double radians);

float floatMod(float x, float m);

float roundtodecimal(float value, unsigned char prec);

#define motorno 4

struct Vector3D {
	float x;
	float y;
	float z;
};

#define MPU_CALIBRATION_COUNT 20
#define MPU_CALIBRATION_TIME 50
#define MPU9250_ADDRESS 0x29
#define MAG_ADDRESS 0x0C
#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

//BNO PID - Robot 1
//If the robot is not correcting fast enough, increase P.
#define HEADING_KP1 0.5
// WARNING: DO NOT USE
#define HEADING_KI1 0
//If the robot does not stop oscillating, increase D.
#define HEADING_KD1 0.06
#define HEADING_MAX_CORRECTION 100

//BNO PID - Robot 2
#define HEADING_KP2 0.5
#define HEADING_KI2 0
#define HEADING_KD2 0.06

//Attack Horizontal PID - Robot 1
#define SPEED_AHP1 1.5
#define SPEED_AHI1 0
#define SPEED_AHD1 0.5

//Attack Vertical PID - Robot 1
#define SPEED_AVP1 1.2
#define SPEED_AVI1 0
#define SPEED_AVD1 0.6
#define ATTACK_MAX_SPEED 20

//Defend Horizontal PID - Robot 1
#define SPEED_DHP1 0.4
#define SPEED_DHI1 0
#define SPEED_DHD1 0.05
#define DEFEND_H_MAX_SPEED 30

//Defend Vertical PID - Robot 1
#define SPEED_DVP1 1.5
#define SPEED_DVI1 0
#define SPEED_DVD1 0.9

//Attack Horizontal PID - Robot 2
#define SPEED_AHP2 1.5
#define SPEED_AHI2 0
#define SPEED_AHD2 0.5

//Attack Vertical PID - Robot 2
#define SPEED_AVP2 1.2
#define SPEED_AVI2 0
#define SPEED_AVD2 0.6
#define ATTACK_MAX_SPEED 20

//Defend Horizontal PID - Robot 2
#define SPEED_DHP2 0.4
#define SPEED_DHI2 0
#define SPEED_DHD2 0.05
#define DEFEND_H_MAX_SPEED 30

//Defend Vertical PID - Robot 2
#define SPEED_DVP2 1.5
#define SPEED_DVI2 0
#define SPEED_DVD2 1
#define DEFEND_V_MAX_SPEED 30

//Camera
#define cameraSerial Serial7
#define CAM_BAUD 115200
#define CAM_PACKET_SIZE 12
#define CAM_START_BYTE 254
#define CAM_CENTRE_X 120
#define CAM_CENTRE_Y 120
#define CAM_NO_DATA 250

//CAMERA STRENGTH
#define CLOSE_BALL 25

//Bluetooth
#define toothSerial Serial1
#define TOOTH_BAUD 9600
#define TOOTH_PACKET_SIZE 6
#define TOOTH_START_BITE 254
#define TOOTH_NO_DATA 255

struct MoveData{
	double angle, correction, line, straight;
	int power;
	bool attacker;
	bool defender;
};

#define MAX_DEFENCE 35
#define CENTER_Y 80
#define TARGET_DISTANCE 11

// Editable
#define ATTACK_GOAL_YELLOW true
#define FindLS false
#define ATTACKER false
#define DEFENDER true
#define ROBOT 2
#endif