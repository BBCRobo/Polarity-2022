#include <Defend.h>
#include <Attack.h>


PID defendhorizontalPID(SPEED_DHP, SPEED_DHI, SPEED_DHD, DEFEND_H_MAX_SPEED);
PID defendverticalPID(SPEED_DVP, SPEED_DVI, SPEED_DVD, DEFEND_V_MAX_SPEED);
Attack Orbit;


/*! @brief Returns the value of the vertical defender PID. */
double Defend::verticalPID(double defenddistance) {
    return defendverticalPID.update(defenddistance, MAX_DEFENCE);
}


/*! @brief Returns the value of the horizontal defender PID. */
double Defend::horizontalPID(double straight) {
    // return -defendhorizontalPID.update(sin(M_PI*straight/180)*balldist, 0);
    return -defendhorizontalPID.update(straight > 180 ? straight - 360 : straight, 0);
}


/*! @brief The power function controls the speed of the robot in different scenarios. */
double Defend::power(bool dalive, bool balive, double straight, double defenddistance, double balldist, double heading) {
    if(dalive) {
        if(balive) {
            if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y && abs(straight > 180 ? straight - 360 : straight) < 20) {
                return 50;
            }
            if((straight > 270 || straight < 90) && balldist > CLOSE_BALL) {
                double forwardbackward = verticalPID(defenddistance);
                double leftright = horizontalPID(straight);
                return sqrt(forwardbackward*forwardbackward + leftright*leftright);
            } else {
                return Orbit.power(straight, balldist);
            }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            int leftright = -horizontalPID(heading);
            return sqrt(forwardbackward*forwardbackward + leftright*leftright);
        }
    } else {
        return 10;
    }
}


/*! @brief The defence function returns the angle the defender should move in to move between the passive ball and the goal. */
double Defend::defence(double defenddistance, double straight) {
    double forwardbackward = verticalPID(defenddistance);
    double leftright = horizontalPID(straight);
    if(leftright == 0) {
        return forwardbackward > 0 ? 0 : 180;
    } else {
        double angle = atan2(forwardbackward, leftright);
        angle = radiansToDegrees(angle);
        return abs(angle-450) > 360 ? abs(angle-90) : abs(angle-450);
    }
}


/*! @brief The angle function controls the logic for the defender and returns the angle the defender should move in. */
double Defend::angle(double straight, double defenddistance, double balldist, bool dalive, bool balive, double heading) {
    if(dalive) {
        if(balive) {
            if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y && abs(straight > 180 ? straight - 360 : straight) < 20) {
                return straight;
            }
            if((straight > 270 || straight < 90) && balldist > CLOSE_BALL) {
                return defence(defenddistance, straight);
            } else {
                return Orbit.orbit(straight, balldist);
            }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            int leftright = -horizontalPID(heading);
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