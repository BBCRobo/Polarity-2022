#include "Arduino.h"

// Polarity 2021 Motor Switch
#define MOTOR_SWITCH 57
#define LS_COUNT 16
// Polarity 2022 LRF MUX
#define MUX_ADDR 0
// Pins for Light Sensors Polarity 2021
#define LS_BINARY_3 5
#define LS_BINARY_2 2
#define LS_BINARY_1 1
#define LS_BINARY_0 0
#define LS_OUTPUT A11
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

// Pins for Motors Polarity 2021
// Front Right
#define MOTOR_ANGLE_FR 45
#define PWM_FR 21
#define INONE_FR 16
#define INTWO_FR 17

// Front Left
#define MOTOR_ANGLE_FL 315
#define PWM_FL 20
#define INONE_FL 15
#define INTWO_FL 39

// Back Right
#define MOTOR_ANGLE_BR 135
#define PWM_BR 23
#define INONE_BR 18
#define INTWO_BR 19

// Back Left
#define MOTOR_ANGLE_BL 225
#define PWM_BL 22
#define INONE_BL 13
#define INTWO_BL 14

// BNO in Wire1