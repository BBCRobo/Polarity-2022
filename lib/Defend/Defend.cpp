#include <Defend.h>

PID defendPID(HEADING_DP, HEADING_DI, HEADING_DD, HEADING_MAX_CORRECTION);
PID sidewaysPID(SPEED_SP, SPEED_SI, SPEED_SD, SPEED_MAX_SPEED);
PID yupPID(SPEED_FP, SPEED_FI, SPEED_FD, SPEED_MAX_SPEED);

double Defend::correction(double hpid, bool dalive, bool defendangle) {
    if(dalive) {
        return -defendPID.update(defendangle, 180);
    } else {
        return hpid;
    }
}

double Defend::yup(double defenddistance) {
    return yupPID.update(defenddistance, MAX_DEFENCE);
}

double Defend::xleft(double straight) {
    return -sidewaysPID.update(straight > 180 ? straight - 360 : straight, 0);
}


double Defend::power(bool dalive, bool balive, double straight, double defenddistance, double balldist) {
    if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y) {
        return 60;
    }
    if(dalive) {
        if(balive) {
            if(straight > 270 || straight < 90) {
                double forwardbackward = yup(defenddistance);
                double leftright = xleft(straight);
                return sqrt(forwardbackward*forwardbackward + leftright*leftright);
            } else if(straight <= 30 || straight >= 330) {
                return 24;
            } else {
                return 18;
            }
        } else {
            int forwardbackward = yup(defenddistance);
            return forwardbackward > 0 ? forwardbackward : abs(forwardbackward);
        }
    } else {
        return 4;
    }
}


double Defend::defence(double defenddistance, double straight) {
    double forwardbackward = yup(defenddistance);
    double leftright = xleft(straight);
    if(leftright == 0) {
        return forwardbackward > 0 ? 0 : 180;
    } else {
        double angle = atan2(forwardbackward, leftright);
        angle = radiansToDegrees(angle);
        return abs(angle-450) > 360 ? abs(angle-90) : abs(angle-450);
    }
}

double Defend::orbit(double straight, double balldist) {
    if(straight <= 30 || straight >= 330) {
        return 0;
    } else if(straight > 180) {
        if(balldist <= CLOSE_BALL) {
            return straight - 90;
        } else {
            return straight - 70;
        }
    } else {
        if(balldist <= CLOSE_BALL) {
            return straight + 90;
        } else {
            return straight + 70;
        }
    }
}

double Defend::angle(double straight, double defenddistance, double balldist, bool dalive, bool balive) {
    if(balldist <= CLOSE_BALL && defenddistance < CENTER_Y) {
        return straight;
    }
    if(dalive) {
        if(balive) {
            if(straight > 270 || straight < 90) {
                return defence(defenddistance, straight);
            } else {
                return orbit(straight, balldist);
            }
        } else {
            int forwardbackward = yup(defenddistance);
            return forwardbackward > 0 ? 0 : 180;
        }
    } else {
        return 0;
    }
}