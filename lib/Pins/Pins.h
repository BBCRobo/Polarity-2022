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
#define LST_PIN_1 31
#define LST_PIN_2 30
#define LST_PIN_3 29
#define LST_PIN_4 28
#define LST_PIN_5 27
#define LST_PIN_6 26
#define LST_PIN_7 25
#define LST_PIN_8 24
#define LST_PIN_9 23
#define LST_PIN_10 22
#define LST_PIN_11 21
#define LST_PIN_12 20
#define LST_PIN_13 19
#define LST_PIN_14 18
#define LST_PIN_15 17
#define LST_PIN_16 16
#define LST_PIN_17 15
#define LST_PIN_18 14
#define LST_PIN_19 13
#define LST_PIN_20 12
#define LST_PIN_21 11
#define LST_PIN_22 10
#define LST_PIN_23 9
#define LST_PIN_24 8
#define LST_PIN_25 7
#define LST_PIN_26 6
#define LST_PIN_27 5
#define LST_PIN_28 4
#define LST_PIN_29 3
#define LST_PIN_30 2
#define LST_PIN_31 1

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

// Kicker
#define KICKER_PIN 26