#include <Defend.h>
#include <Attack.h>


PID defendhorizontalPID(SPEED_DHP, SPEED_DHI, SPEED_DHD, DEFEND_MAX_SPEED);
PID defendverticalPID(SPEED_DVP, SPEED_DVI, SPEED_DVD, DEFEND_MAX_SPEED);
Attack Orbit;


/*! @brief Returns the value of the vertical defender PID. */
double Defend::verticalPID(double defenddistance) {
    return defendverticalPID.update(defenddistance, MAX_DEFENCE);
}


/*! @brief Returns the value of the horizontal defender PID. */
double Defend::horizontalPID(double straight) {
    return -defendhorizontalPID.update(straight > 180 ? straight - 360 : straight, 0);
}


/*! @brief The power function controls the speed of the robot in different scenarios. */
double Defend::power(bool dalive, bool balive, double straight, double defenddistance, double balldist) {
    if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y) {
        return 50;
    }
    if(dalive) {
        if(balive) {
            if(straight > 270 || straight < 90) {
                double forwardbackward = verticalPID(defenddistance);
                double leftright = horizontalPID(straight);
                leftright = 0;
                return sqrt(forwardbackward*forwardbackward + leftright*leftright);
            } else if(straight <= 30 || straight >= 330) {
                return 30;
            } else {
                return 20;
            }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            return forwardbackward > 0 ? forwardbackward : abs(forwardbackward);
        }
    } else {
        return 20;
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
double Defend::angle(double straight, double defenddistance, double balldist, bool dalive, bool balive) {
    if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y) {
        return straight;
    }
    if(dalive) {
        if(balive) {
            // if(straight > 270 || straight < 90) {
            return defence(defenddistance, straight);
            // } else {
            //     return Orbit.orbit(straight, balldist);
            // }
        } else {
            int forwardbackward = verticalPID(defenddistance);
            return forwardbackward > 0 ? 0 : 180;
        }
    } else {
        return 180;
    }
}