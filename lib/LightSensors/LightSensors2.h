#ifndef LIGHTSENSORS2_H
#define LIGHTSENSORS2_H

#include <Pins.h>

class LightSensor2 {
public:
    LightSensor2() {};
    void init();
    double DirectionOfLine(float orientation);
    int read(int no);
private:
    int green[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int totalno;
    int total;
    int direction;
    int location;
    float origorientation;
    float offby;
    float fieldlineangle = -1;
    float fieldlinesize;
    bool wrong;
    int orig;
    bool onfield = true;
    double direc2[LS_COUNT] = {180, 191.25, 202.5, 213.75, 225, 236.25, 247.5, 258.75, 270, 281.25, 292.5, 303.75, 315, 326.25, 337.5, 348.75, 360, 371.25, 382.5, 393.75, 405, 416.25, 427.5, 438.75, 450, 461.25, 472.5, 483.75, 495, 506.25, 517.5, 528.75};
    double direc[LS_COUNT] = {180, 191.25, 202.5, 213.75, 225, 236.25, 247.5, 258.75, 270, 281.25, 292.5, 303.75, 315, 326.25, 337.5, 348.75, 0, 11.25, 22.5, 33.75, 45, 56.25, 67.5, 78.75, 90, 101.25, 112.5, 123.75, 135, 146.25, 157.5, 168.75};
    int lspins[LS_COUNT] = {LST_PIN_0, LST_PIN_1, LST_PIN_2, LST_PIN_3, LST_PIN_4, LST_PIN_5, LST_PIN_6, LST_PIN_7, LST_PIN_8, LST_PIN_9, LST_PIN_10, LST_PIN_11, LST_PIN_12, LST_PIN_13, LST_PIN_14, LST_PIN_15, LST_PIN_16, LST_PIN_17, LST_PIN_18, LST_PIN_19, LST_PIN_20, LST_PIN_21, LST_PIN_22, LST_PIN_23, LST_PIN_24, LST_PIN_25, LST_PIN_26, LST_PIN_27, LST_PIN_28, LST_PIN_29, LST_PIN_30, LST_PIN_31};
};

#endif