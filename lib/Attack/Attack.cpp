#include <Attack.h>

double Attack::orbit(double straight, double balldist) {
    int returning = 0;
    if(straight > 10 && straight < 350) {
        if(straight >= 180) {
            // returning = straight - (45*cos(M_PI*balldist/160)+45);
            returning = straight - pow(euler, 0.02*(225-balldist));
        } else {
            // returning = straight + (45*cos(M_PI*balldist/160)+45);
            returning = straight + pow(euler, 0.02*(225-balldist));
        }
    }
    if(balldist > 80) {
        returning = straight;
    }
    return returning;
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