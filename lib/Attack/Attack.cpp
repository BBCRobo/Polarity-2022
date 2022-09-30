#include <Attack.h>


PID attackhorizontalPID1(SPEED_AHP1, SPEED_AHI1, SPEED_AHD1, ATTACK_MAX_SPEED);
PID attackverticalPID1(SPEED_AVP1, SPEED_AVI1, SPEED_AVD1, ATTACK_MAX_SPEED);
PID attackhorizontalPID2(SPEED_AHP2, SPEED_AHI2, SPEED_AHD2, ATTACK_MAX_SPEED);
PID attackverticalPID2(SPEED_AVP2, SPEED_AVI2, SPEED_AVD2, ATTACK_MAX_SPEED);


/*! @brief Calculates the angle the robot should move in based on 2 PIDs (which give values based on the ball position). */
double Attack::orbit(double straight, double balldist, double attackdistance) {
    if(abs(straight > 180 ? straight - 360 : straight) < 15) {
        return 0;
    }
    float horizontal_move = -attackhorizontalPID2.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
    float vertical_move = -attackverticalPID2.update(cos(M_PI*straight/180)*balldist, 20);//abs(straight > 180 ? straight - 360 : straight) > 90 ? 0 : 10);
    if(ROBOT == 1) {
        horizontal_move = -attackhorizontalPID1.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
        vertical_move = -attackverticalPID1.update(cos(M_PI*straight/180)*balldist, 20);//abs(straight > 180 ? straight - 360 : straight) > 90 ? 0 : 10);
    }
    double returning = radiansToDegrees(atan2(horizontal_move, vertical_move));
    if(returning < 0) {
        returning += 360;
    }
    // if(attackdistance != -1 && attackdistance < 30 && returning > 180 ? abs(returning - 360) < 90 : abs(returning) < 90) {
    //     returning = -1;
    // }
    return returning;
}


/*! @brief The power function determines the speed of the robot based on the 2 movement PIDs. */
double Attack::power(double straight, double balldist, bool balive) {
    if(balive) {
        if(abs(straight > 180 ? straight - 360 : straight) < 15) {
            return 35;
        }
        float horizontal_move = -attackhorizontalPID2.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
        float vertical_move = -attackverticalPID2.update(cos(M_PI*straight/180)*balldist, 20);//abs(eyes.straight > 180 ? eyes.straight - 360 : eyes.straight) > 90 ? 0 : 10);
        if(ROBOT == 1) {
            horizontal_move = -attackhorizontalPID1.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
            vertical_move = -attackverticalPID1.update(cos(M_PI*straight/180)*balldist, 20);//abs(eyes.straight > 180 ? eyes.straight - 360 : eyes.straight) > 90 ? 0 : 10);
        }
        return sqrt(vertical_move*vertical_move + horizontal_move*horizontal_move);
    } else {
        return 20;
    }
}


/*! @brief The circle function moves the robot in a circle if it cannot see the line. */
double Attack::circle() {
    unsigned long current_time = micros();
    if(last_circle_accl == -1) {
        last_circle_accl = micros();
        current_angle = 270;
        return 270;
    }
    float returning_angle = floatMod(current_angle + acceleration_rate*(current_time - last_circle_accl), 360);
    current_angle = returning_angle;
    last_circle_accl = micros();
    return returning_angle;
}


/*! @brief The angle function runs the logic for the attacker. It will return the angle that the robot will move in. */
double Attack::angle(bool balive, double balldist, double straight, int pos_x, int pos_y, double attackdistance) {
    if(balive) {
        // for(int i = 0; i < 4; i++) {
        //     checked_neutral_point[i] = false;
        // }
        last_circle_accl = -1;
        current_angle = -1;
        return orbit(straight, balldist, attackdistance);
    } else {
        return circle();
        // return -1;
        // if(checked_neutral_point[0] && checked_neutral_point[1] && checked_neutral_point[2] && checked_neutral_point[3]) {
        //     checked_neutral_point[2] = false;
        //     checked_neutral_point[3] = false;
        // }
        // int closest_np = 0;
        // int closest_distance = sqrt(pow(neutral_point_pos[closest_np][0]-pos_x, 2) + pow(neutral_point_pos[closest_np][1]-pos_y, 2));
        // for(int i = 0; i < 4; i++) {
        //     int point_distance = sqrt(pow(neutral_point_pos[i][0]-pos_x, 2) + pow(neutral_point_pos[i][1]-pos_y, 2));
        //     if(point_distance < closest_distance && point_distance > 50 && checked_neutral_point[i] == false) {
        //         closest_np = i;
        //         closest_distance = point_distance;
        //     } else if(point_distance < 50) {
        //         checked_neutral_point[i] = true;
        //     }
        // }
        // return floatMod(atan2((neutral_point_pos[closest_np][0]-pos_x),(pos_y - neutral_point_pos[closest_np][1]))*180/M_PI, 360);
    }
    return -1;
}