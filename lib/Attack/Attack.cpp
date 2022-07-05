#include <Attack.h>


PID attackhorizontalPID(SPEED_AHP, SPEED_AHI, SPEED_AHD, ATTACK_MAX_SPEED);
PID attackverticalPID(SPEED_AVP, SPEED_AVI, SPEED_AVD, ATTACK_MAX_SPEED);


/*! @brief Calculates the angle the robot should move in based on 2 PIDs (which give values based on the ball position). */
double Attack::orbit(double straight, double balldist) {
    if(abs(straight > 180 ? straight - 360 : straight) < 10 && balldist < CLOSE_BALL + 5) {
        return 0;
    }
    float horizontal_move = -attackhorizontalPID.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
    float vertical_move = -attackverticalPID.update(cos(M_PI*straight/180)*balldist, 15);//abs(straight > 180 ? straight - 360 : straight) > 90 ? 0 : 10);
    double returning = radiansToDegrees(atan2(horizontal_move, vertical_move));
    if(returning < 0) {
        returning += 360;
    }
    return returning;
}


/*! @brief The power function determines the speed of the robot based on the 2 movement PIDs. */
double Attack::power(double straight, double balldist) {
    if(abs(straight > 180 ? straight - 360 : straight) < 10 && balldist < CLOSE_BALL + 5) {
        return 30;
    }
    float horizontal_move = -attackhorizontalPID.update(sin(M_PI*straight/180)*balldist, abs(straight > 180 ? straight - 360 : straight) > 90 ? (straight > 180 ? (straight - 360)/10 : (straight)/10) : 0);
    float vertical_move = -attackverticalPID.update(cos(M_PI*straight/180)*balldist, 15);//abs(eyes.straight > 180 ? eyes.straight - 360 : eyes.straight) > 90 ? 0 : 10);
    return sqrt(vertical_move*vertical_move + horizontal_move*horizontal_move);
}


/*! @brief The angle function runs the logic for the attacker. It will return the angle that the robot will move in. */
double Attack::angle(bool balive, double balldist, double straight) {
    if(balive) {
        return orbit(straight, balldist);
    } else {
        return -1;
    }
    // if(aalive) {
    //     if(dalive) {
    //         if(attackdistance < CENTER_Y) {
    //             if(defendangle < 190) {
    //                 return 135;
    //             } else if(defendangle > 190) {
    //                 return 225;
    //             } else {
    //                 return 180;
    //             }
    //         } else if(attackdistance > CENTER_Y) {
    //             if(defendangle < 190) {
    //                 return 45;
    //             } else if(defendangle > 190) {
    //                 return 315;
    //             } else {
    //                 return 0;
    //             }
    //         } else {
    //             if(defendangle < 190) {
    //                 return 90;
    //             } else if(defendangle > 190) {
    //                 return 270;
    //             } else {
    //                 return -1;
    //             }
    //         }
    //     } else {
    //         if(attackdistance < CENTER_Y) {
    //             return 180;
    //         } else if(attackdistance > CENTER_Y) {
    //             return 0;
    //         } else {
    //             return -1;
    //         }
    //     }
    // } else if(dalive) {
    //     return 0;
    // }
    return -1;
}