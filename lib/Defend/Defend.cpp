#include <Defend.h>
#include <Attack.h>
#include <Kicker.h>


PID defendhorizontalPID1(SPEED_DHP1, SPEED_DHI1, SPEED_DHD1, DEFEND_H_MAX_SPEED);
PID defendverticalPID1(SPEED_DVP1, SPEED_DVI1, SPEED_DVD1, DEFEND_V_MAX_SPEED);
PID defendhorizontalPID2(SPEED_DHP2, SPEED_DHI2, SPEED_DHD2, DEFEND_H_MAX_SPEED);
PID defendverticalPID2(SPEED_DVP2, SPEED_DVI2, SPEED_DVD2, DEFEND_V_MAX_SPEED);
Attack Orbit;
Kicker kick;


void Defend::kick_init() {
    kick.init();
}

bool Defend::check_kick() {
    return kick.check_LDR();
}


/*! @brief Returns the value of the vertical defender PID. */
double Defend::verticalPID(double defenddistance) {
    return defendverticalPID1.update(defenddistance, MAX_DEFENCE);
}


/*! @brief Returns the value of the horizontal defender PID. */
double Defend::horizontalPID(double straight) {
    // return -defendhorizontalPID.update(sin(M_PI*straight/180)*balldist, 0);
    if(ROBOT == 1) {
        return -defendhorizontalPID1.update(straight > 180 ? straight - 360 : straight, 0);
    } else {
        return -defendhorizontalPID2.update(straight > 180 ? straight - 360 : straight, 0);
    }
}


/*! @brief The power function controls the speed of the robot in different scenarios. */
double Defend::power(bool dalive, bool balive, double straight, double defenddistance, double balldist, double heading) {
    if(dalive) {
        if(balive) {
            if(balldist <= CLOSE_BALL && abs(straight > 180 ? straight - 360 : straight) < 20 && defenddistance < CENTER_Y) {
                if(kick.check_LDR()) {
                    return 60;
                }
                return 30;
            }
            if((straight > 270 || straight < 90)) {
                double forwardbackward = verticalPID(defenddistance);
                double leftright = horizontalPID(straight);
                // if(heading >= 60 && heading < 180 && leftright > 0) {
                //     leftright = -10;
                // } else if(heading <= 300 && heading > 180 && leftright < 0) {
                //     leftright = 10;
                // }
                return sqrt(forwardbackward*forwardbackward + leftright*leftright);
            } else {
                return Orbit.power(straight, balldist, balive);
            }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            int leftright = -horizontalPID(heading);
            return sqrt(forwardbackward*forwardbackward + leftright*leftright);
        }
    } else {
        return 5;
    }
}


/*! @brief The defence function returns the angle the defender should move in to move between the passive ball and the goal. */
double Defend::defence(double defenddistance, double straight, double heading) {
    double forwardbackward = verticalPID(defenddistance);
    double leftright = horizontalPID(straight);
    // if(heading >= 60 && heading < 180 && leftright > 0) {
    //     leftright = -10;
    // } else if(heading <= 300 && heading > 180 && leftright < 0) {
    //     leftright = 10;
    // }
    if(leftright == 0) {
        return forwardbackward > 0 ? 0 : 180;
    } else {
        double angle = atan2(forwardbackward, leftright);
        angle = radiansToDegrees(angle);
        return abs(angle-450) > 360 ? abs(angle-90) : abs(angle-450);
    }
}


/*! @brief The angle function controls the logic for the defender and returns the angle the defender should move in. */
double Defend::angle(double straight, double defenddistance, double balldist, bool dalive, bool balive, double heading, double attackdistance) {
    if(dalive) {
        if(balive) {
            if(balldist <= CLOSE_BALL && abs(straight > 180 ? straight - 360 : straight) < 20 && defenddistance < CENTER_Y) {
                return straight;
            }
            if((straight > 270 || straight < 90) && balldist > CLOSE_BALL) {
                return defence(defenddistance, straight, heading);
            } else {
                return Orbit.orbit(straight, balldist, attackdistance);
            }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            int leftright = -horizontalPID(heading);
            leftright = 0;
            if(leftright == 0) {
                return forwardbackward > 0 ? 0 : 180;
            } else {
                double angle = atan2(forwardbackward, leftright);
                angle = radiansToDegrees(angle);
                return abs(angle-450) > 360 ? abs(angle-90) : abs(angle-450);
            }
        }
    } else {
        return 180;
    }
}