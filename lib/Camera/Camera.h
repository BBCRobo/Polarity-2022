#ifndef CAMERA_H
#define CAMERA_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>

class Camera {
public:
    Camera() {};
    void init();
    void read();
    bool aalive;
    bool dalive;
    bool balive;
    double attackangle;
    double defendangle;
    int attackdistance;
    int defenddistance;
    double straight;
    int balldist;
private:
    int camBuffer[CAM_PACKET_SIZE - 1];
    double calcangle(int _x, int _y);
    double calcdistance(int _x, int _y);
    double calcorbit();
};

#endif