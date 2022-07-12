#include <Bluetooth.h>


/*! @brief Initialises the Bluetooth*/
void Bluetooth::init() {
    toothSerial.begin(TOOTH_BAUD);
    last_received_time = micros();
    last_sent_time = micros();
}


/*! @brief Controls the Bluetooth logic. Determines when the teensy reads and writes to the Bluetooth Module.*/
void Bluetooth::update(int robot_location_x, int robot_location_y, int ball_location_x, int ball_location_y) {
    unsigned long current_time = micros();
    if(current_time - last_sent_time > 250000) {
        write(robot_location_x, robot_location_y, ball_location_x, ball_location_y);
    }
    bool read_data = read();
    if(read_data) {
        last_received_time = micros();
    }
    current_time = micros();
    connection = current_time - last_received_time > 2000000 ? false : true;
}


/*! @brief Attempts to read the Bluetooth Module if the Serial has more than one full packet.*/
bool Bluetooth::read() {
    while(toothSerial.available() >= TOOTH_PACKET_SIZE) {
        if(toothSerial.read() == TOOTH_START_BITE) {
            for(int i = 0; i < TOOTH_PACKET_SIZE - 1; i++) {
                toothBuffer[i] = toothSerial.read();
            }
            other_robot_location[0] = (toothBuffer[0] == TOOTH_NO_DATA ? -1 : toothBuffer[0]);
            other_robot_location[1] = (toothBuffer[0] == TOOTH_NO_DATA ? -1 : toothBuffer[1]);
            other_ball_location[0] = (toothBuffer[2] == TOOTH_NO_DATA ? -1 : toothBuffer[2]);
            other_ball_location[1] = (toothBuffer[2] == TOOTH_NO_DATA ? -1 : toothBuffer[3]);
            return true;
        }
    }
    return false;
}


/*! @brief Writes data to the Bluetooth Module.*/
void Bluetooth::write(int robot_location_x, int robot_location_y, int ball_location_x, int ball_location_y) {
    toothSerial.write(TOOTH_START_BITE);
    toothSerial.write(robot_location_x);
    toothSerial.write(robot_location_y);
    toothSerial.write(ball_location_x);
    toothSerial.write(ball_location_y);
    last_sent_time = micros();
}