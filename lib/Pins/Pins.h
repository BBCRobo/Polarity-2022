#include "Arduino.h"

// Polarity 2022
#define LS_COUNT 32
// Light Sensor MUXs
#define LS_BINARY_3 23
#define LS_BINARY_2 22
#define LS_BINARY_1 21
#define LS_BINARY_0 20
#define LS_OUTPUT_1 A11
#define LS_OUTPUT_2 A10
// Light Sensors - Check that the sensors are the right way round on the PCB
#define LST_PIN_0 0
#define LST_PIN_1 1
#define LST_PIN_2 2
#define LST_PIN_3 3
#define LST_PIN_4 4
#define LST_PIN_5 5
#define LST_PIN_6 6
#define LST_PIN_7 7
#define LST_PIN_8 8
#define LST_PIN_9 9
#define LST_PIN_10 10
#define LST_PIN_11 11
#define LST_PIN_12 12
#define LST_PIN_13 13
#define LST_PIN_14 14
#define LST_PIN_15 15

// Pins for Motors
// Front Right
#define MOTOR_ANGLE_FR 45
#define PWM_FR 10
#define INONE_FR 5
#define INTWO_FR 4

// Front Left
#define MOTOR_ANGLE_FL 315
#define PWM_FL 14
#define INONE_FL 6
#define INTWO_FL 9

// Back Right
#define MOTOR_ANGLE_BR 135
#define PWM_BR 12
#define INONE_BR 3
#define INTWO_BR 2

// Back Left
#define MOTOR_ANGLE_BL 225
#define PWM_BL 11
#define INONE_BL 8
#define INTWO_BL 7

// BNO in Wire1

// LRFs
// LRF in Wire0
#define LRF_1 30
#define LRF_2 31
#define LRF_3 32
#define LRF_4 33