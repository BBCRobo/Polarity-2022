#include <Attack.h>

PID attackPID(HEADING_AP, HEADING_AI, HEADING_AD, HEADING_MAX_CORRECTION);

double Attack::correction(double hpid, double attackangle, bool aalive, double balldist) {
    double apid = -attackPID.update(attackangle > 180 ? attackangle - 360 : attackangle, 0);
    if(aalive) {
        if(balldist > CLOSE_BALL) {
            return sqrt(apid*apid + hpid*hpid);
        } else {
            return apid;
        }
    } else {
        return hpid;
    }
}

double Attack::orbit(double straight, double balldist) {
    if(straight <= 30 || straight >= 330) {
        return 0;
    } else if(straight > 180) {
        if(balldist <= CLOSE_BALL) {
            return (straight - 90);
        } else {
            return (straight - 70);
        }
    } else {
        if(balldist <= CLOSE_BALL) {
            return (straight + 90);
        } else {
            return (straight + 70);
        }
    }
}

double Attack::angle(bool aalive, bool balive, bool dalive, double attackdistance, double defendangle, double balldist, double straight) {
    if(balive) {
        return orbit(straight, balldist);
    } else if(aalive) {
        if(dalive) {
            if(attackdistance < CENTER_Y) {
                if(defendangle < 190) {
                    return 135;
                } else if(defendangle > 190) {
                    return 225;
                } else {
                    return 180;
                }
            } else if(attackdistance > CENTER_Y) {
                if(defendangle < 190) {
                    return 45;
                } else if(defendangle > 190) {
                    return 315;
                } else {
                    return 0;
                }
            } else {
                if(defendangle < 190) {
                    return 90;
                } else if(defendangle > 190) {
                    return 270;
                } else {
                    return -1;
                }
            }
        } else {
            if(attackdistance < CENTER_Y) {
                return 180;
            } else if(attackdistance > CENTER_Y) {
                return 0;
            } else {
                return -1;
            }
        }
    } else if(dalive) {
        return 0;
    }
    return -1;
}