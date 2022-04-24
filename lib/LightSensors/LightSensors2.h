#ifndef LIGHTSENSORS2_H
#define LIGHTSENSORS2_H

#include <Pins.h>

class LightSensor2 {
public:
    LightSensor2() {};
    void init();
    double DirectionOfLine(float orientation);
    int prev;
    int read(int no);
private:
    int green[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
    double direc2[LS_COUNT] = {180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5, 360, 382.5, 405, 427.5, 450, 472.5, 495, 517.5};
    double direc[LS_COUNT] = {180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5, 0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5};
    int lspins[LS_COUNT] = {LST_PIN_0, LST_PIN_1, LST_PIN_2, LST_PIN_3, LST_PIN_4, LST_PIN_5, LST_PIN_6, LST_PIN_7, LST_PIN_8, LST_PIN_9, LST_PIN_10, LST_PIN_11, LST_PIN_12, LST_PIN_13, LST_PIN_14, LST_PIN_15};
};

#endif