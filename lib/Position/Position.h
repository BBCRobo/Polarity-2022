#ifndef POSITION_H
#define POSITION_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>
#include <LRF.h>

class Position {
public:
    Position() {};
    void init();
    void read_lrfs();
    void update(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, double straight, double balldist, bool aalive, bool dalive);
    bool check_x();
    bool check_y();
    void lrf_distances(float orientation);
    void goal_position(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, bool aalive, bool dalive);
    void ball_position(float orientation, double straight, double balldist);
    int fit_to_grid(float x, float y);
    int position = -1;
    float pos_x = -1000;
    float pos_y = -1000;
    float ball_pos_x = -1000;
    float ball_pos_y = -1000;
private:
    float a_x = -1000;
    float a_y = -1000;
    float d_x = -1000;
    float d_y = -1000;
    float b_l = -1000;
    float f_l = -1000;
    float l_l = -1000;
    float r_l = -1000;
    float goal_pos_x = -1000;
    float goal_pos_y = -1000;
    int g_position = -1;
};
#endif