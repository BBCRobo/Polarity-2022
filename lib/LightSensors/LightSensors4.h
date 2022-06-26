#ifndef LIGHTSENSORS4_H
#define LIGHTSENSORS4_H

#include <Pins.h>

class LightSensor4 {
public:
    LightSensor4() {};
    void init();
    double DirectionOfLine(float orientation, int position);
    double AngleBetweenClusters(float centre1, float centre2);
    int read(int no);
    void FindGreen();
    void FindWhite();
    int lsdebug[LS_COUNT][2] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    int green[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
private:
    int totalno;
    double direction;
    bool offby;
    int location;
    double expected;
    double prev;
    bool clustering;
    int lspins[LS_COUNT] = {LST_PIN_0, LST_PIN_1, LST_PIN_2, LST_PIN_3, LST_PIN_4, LST_PIN_5, LST_PIN_6, LST_PIN_7, LST_PIN_8, LST_PIN_9, LST_PIN_10, LST_PIN_11, LST_PIN_12, LST_PIN_13, LST_PIN_14, LST_PIN_15, LST_PIN_16, LST_PIN_17, LST_PIN_18, LST_PIN_19, LST_PIN_20, LST_PIN_21, LST_PIN_22, LST_PIN_23, LST_PIN_24, LST_PIN_25, LST_PIN_26, LST_PIN_27, LST_PIN_28, LST_PIN_29, LST_PIN_30, LST_PIN_31};
};

#endif