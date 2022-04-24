#ifndef COMMON_H
#define COMMON_H

#include <math.h>

#define TO_RADIANS 0.01745329251994329576923690768489

double degreesToRandians(double degrees);

#define TO_DEGREES 57.295779513082320876798154814105

#define euler 2.7182818284590452353602874713527

double radiansToDegrees(double radians);

float floatMod(float x, float m);

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

//BNO PID
//If the robot is not correcting fast enough, increase P.
#define HEADING_KP 0.2
// WARNING: DO NOT USE
#define HEADING_KI 0
//If the robot does not stop oscillating, increase D.
#define HEADING_KD 0.03
#define HEADING_MAX_CORRECTION 20

//Attack PID
#define HEADING_AP 0.2
#define HEADING_AI 0
#define HEADING_AD 0.03

//Defend PID
#define HEADING_DP 0.2
#define HEADING_DI 0
#define HEADING_DD 0.03

//Sideways PID
#define SPEED_SP 0.5
#define SPEED_SI 0
#define SPEED_SD 0.02

//Forwards Backwards (YUP) PID
#define SPEED_FP 0.5
#define SPEED_FI 0
#define SPEED_FD 0
#define SPEED_MAX_SPEED 20

#define ROBOT 2

//Camera
#define cameraSerial Serial7
#define CAM_BAUD 115200
#define CAM_PACKET_SIZE 7
#define CAM_START_BYTE 254
#define CAM_CENTRE_X 120
#define CAM_CENTRE_Y 120
#define CAM_NO_DATA 250

//CAMERA STRENGTH
#define FAR_BALL 50
//CAMERA STRENGTH
#define CLOSE_BALL 40

struct MoveData{
	double angle, correction, line, straight;
	int power;
	bool surge;
	int nosee;
};

#define MAX_DEFENCE 40
#define CENTER_Y 56
#define CENTER_X 0
#define LRF_MUX_ENABLE 0

#endif