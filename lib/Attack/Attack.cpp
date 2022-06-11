#include <Attack.h>

PID attackPID(HEADING_AP, HEADING_AI, HEADING_AD, HEADING_A_MAX_CORRECTION);

double Attack::correction(double hpid, double attackangle, bool aalive, double balldist) {
    double apid = -attackPID.update(attackangle > 180 ? attackangle - 360 : attackangle, 0);
    if(aalive) {
    //     if(balldist > CLOSE_BALL) {
    //         return sqrt(apid*apid + hpid*hpid);
    //     } else {
        return apid;
    //     }
    } //else {
    //     return hpid;
    // }
    // return hpid;
}

double Attack::orbit(double straight, double balldist) {
    // //This works but is bad
    // if(straight <= 10 || straight >= 350) {
    //     return 0;
    // } else if(straight > 270) {
    //     int returning = (180*sin(M_PI*(straight-360)/180)-straight)*ORBIT_CONSTANT/balldist+straight;
    //     return returning < 0 ? returning + 360 : returning;
    // } else if(straight > 180) {
    //     int returning = (45*sin(M_PI*(straight-405)/90)-225-straight)*ORBIT_CONSTANT/balldist+straight;
    //     return returning < 0 ? returning + 360 : returning;
    // } else if(straight < 90) {
    //     int returning = (180*sin(M_PI*(straight)/180)-straight)*ORBIT_CONSTANT/balldist+straight;
    //     return returning;
    // } else {
    //     int returning = (45*sin(M_PI*(straight-135)/90)+225-straight)*ORBIT_CONSTANT/balldist+straight;
    //     return returning;
    // }
    //Next attempt
    // Serial.print(straight);
    // Serial.print(" ");
    // Serial.print(balldist);
    // Serial.print(" ");
    if(straight <= 10 || straight >= 350) {
        // Serial.println(0);
        return 0;
    }
    if(straight >= 180) {
        int returning = straight - asin(TARGET_DISTANCE*sin(degreesToRandians(360-straight))/sqrt(pow(balldist,2)*pow(TARGET_DISTANCE,2)-2*balldist*TARGET_DISTANCE*cos(degreesToRandians(360-straight)))) - (1+240.824/(balldist-8.29412)+0.5*(straight - asin(TARGET_DISTANCE*sin(degreesToRandians(360-straight))/sqrt(pow(balldist,2)*pow(TARGET_DISTANCE,2)-2*balldist*TARGET_DISTANCE*cos(degreesToRandians(360-straight))))))/2;
        // Serial.println(returning);
        return returning;
    } else {
        int returning = straight + asin(TARGET_DISTANCE*sin(degreesToRandians(straight))/sqrt(pow(balldist,2)*pow(TARGET_DISTANCE,2)-2*balldist*TARGET_DISTANCE*cos(degreesToRandians(straight)))) + (1+240.824/(balldist-8.29412)+0.5*(straight + asin(TARGET_DISTANCE*sin(degreesToRandians(straight))/sqrt(pow(balldist,2)*pow(TARGET_DISTANCE,2)-2*balldist*TARGET_DISTANCE*cos(degreesToRandians(straight))))))/2;
        // Serial.println(returning);
        return returning;
    }
}

double Attack::angle(bool aalive, bool balive, bool dalive, double attackdistance, double defendangle, double balldist, double straight) {
    if(balive) {
        return orbit(straight, balldist);
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