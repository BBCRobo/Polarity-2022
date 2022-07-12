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
#define HEADING_KP2 0.2
#define HEADING_KI2 0
#define HEADING_KD2 0

//Attack Horizontal PID
#define SPEED_AHP 1.5
#define SPEED_AHI 0
#define SPEED_AHD 0.5

//Attack Vertical PID
#define SPEED_AVP 1
#define SPEED_AVI 0
#define SPEED_AVD 0.05
#define ATTACK_MAX_SPEED 20

//Defend Horizontal PID
#define SPEED_DHP 0.4
#define SPEED_DHI 0
#define SPEED_DHD 0.05
#define DEFEND_H_MAX_SPEED 30

//Defend Vertical PID
#define SPEED_DVP 1.5
#define SPEED_DVI 0
#define SPEED_DVD 0.9
#define DEFEND_V_MAX_SPEED 50

//Camera
#define cameraSerial Serial7
#define CAM_BAUD 115200
#define CAM_PACKET_SIZE 12
#define CAM_START_BYTE 254
#define CAM_CENTRE_X 120
#define CAM_CENTRE_Y 120
#define CAM_NO_DATA 250

//CAMERA STRENGTH
#define CLOSE_BALL 15

//Bluetooth
#define toothSerial Serial1
#define TOOTH_BAUD 38400
#define TOOTH_PACKET_SIZE 6
#define TOOTH_START_BITE 254
#define TOOTH_NO_DATA 255

struct MoveData{
	double angle, correction, line, straight;
	int power;
	bool attacker;
	bool defender;
};

//LRFs
#define LRF_MUX_ENABLE 0

#define MAX_DEFENCE 35
#define CENTER_Y 75
#define TARGET_DISTANCE 11

// Editable
#define ATTACK_GOAL_YELLOW true
#define FindLS false
#define ATTACKER true
#define DEFENDER false
#define ROBOT 2

#endif