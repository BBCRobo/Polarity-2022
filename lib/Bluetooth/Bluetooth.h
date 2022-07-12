#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Pins.h>
#include <Arduino.h>
#include <Common.h>

class Bluetooth {
public:
    Bluetooth() {};
    void init();
    void update(int robot_location_x, int robot_location_y, int ball_location_x, int ball_location_y);
    bool read();
    void write(int robot_location_x, int robot_location_y, int ball_location_x, int ball_location_y);
    int other_robot_location[2] = {-1,-1};
    int other_ball_location[2] = {-1,-1};
    bool connection;
private:
    int toothBuffer[TOOTH_PACKET_SIZE - 1];
    unsigned long last_sent_time;
    unsigned long last_received_time;
};
#endif