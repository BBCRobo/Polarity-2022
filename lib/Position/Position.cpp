#include <Position.h>


LRF laser;


/*! @brief Allows the initialisation of the lrfs from the main loop. */
void Position::init() {
    laser.init();
    last_lrf_read = micros();
}


/*! @brief Allows the lrfs to be read from the main loop. */
void Position::read_lrfs() {
    unsigned long current_time = micros();
    if(current_time - last_lrf_read < 5000) {
        laser.read_all();
        last_lrf_read = micros();
    }
}


/*! @brief Runs the logic of the positioning system. */
void Position::update(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, double straight, double balldist, bool aalive, bool dalive) {
    // lrf_distances(orientation);
    goal_position(orientation, attackangle, attackdistance, defendangle, defenddistance, aalive, dalive);
    pos_x = goal_pos_x;
    pos_y = goal_pos_y;
    // if(g_position - g_position%10 < 2) {
    //     pos_x = l_l == -1000 ? goal_pos_x : l_l;
    // } else if(g_position - g_position%10 > 3) {
    //     pos_x = r_l == -1000 ? goal_pos_x : r_l;
    // } else if(check_x()) {
    //     pos_x = 0.5*r_l + 0.5*l_l;
    // } else {
    //     pos_x = goal_pos_x;
    // }
    // if(g_position%10 < 2) {
    //     pos_y = f_l == -1000 ? goal_pos_y : f_l;
    // } else if(g_position%10 > 5) {
    //     pos_y = b_l == -1000 ? goal_pos_y : b_l;
    // } else if(check_y()) {
    //     pos_y = 0.5*b_l + 0.5*f_l;
    // } else {
    //     pos_y = goal_pos_y;
    // }
    // position = fit_to_grid(pos_x, pos_y);
    position = g_position;
    ball_position(orientation, straight, balldist);
    // Serial.print(orientation);
    // Serial.print(" goals: ");
    // Serial.print(attackdistance);
    // Serial.print(" ");
    // Serial.print(a_x);
    // Serial.print(" ");
    // Serial.print(a_y);
    // Serial.print(" ");
    // Serial.print(defenddistance);
    // Serial.print(" ");
    // Serial.print(d_x);
    // Serial.print(" ");
    // Serial.print(d_y);
    // Serial.print(" ");
    // Serial.print("pos's: ");
    // Serial.print(g_position);
    // Serial.print(" ");
}


/*! @brief Converts the lrf values to x and y distances. */
void Position::lrf_distances(float orientation) {
    if(abs(orientation - 360) < 45 || abs(orientation - 0) < 45) {
        if(laser.lrf_init[0]) {
            f_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[1]) {
            b_l = 205 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[2]) {
            l_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[3]) {
            r_l = 157 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180)/10;
        }
    } else if(abs(orientation - 270) <= 45) {
        if(laser.lrf_init[0]) {
            l_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[1]) {
            r_l = 205 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[2]) {
            b_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[3]) {
            f_l = 157 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180)/10;
        }
    } else if(abs(orientation - 180) < 45) {
        if(laser.lrf_init[0]) {
            b_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[1]) {
            f_l = 205 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[2]) {
            r_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[3]) {
            l_l = 157 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180)/10;
        }
    } else {
        if(laser.lrf_init[0]) {
            r_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[1]) {
            l_l = 205 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[2]) {
            f_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180)/10;
        }
        if(laser.lrf_init[3]) {
            b_l = 157 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180)/10;
        }
    }
}


/*! @brief Determines the position of the robot based on the goal. */
void Position::goal_position(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, bool aalive, bool dalive) {
    if(aalive) {
        a_x = 78.55-sin(floatMod(attackangle + orientation, 360)*M_PI/180)*attackdistance;
        a_y = 27+cos(floatMod(attackangle + orientation, 360)*M_PI/180)*attackdistance;
        if(dalive) {
            d_x = 78.5-sin(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            d_y = 178+cos(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            goal_pos_x = 0.5*a_x + 0.5*d_x;
            goal_pos_y = 0.5*a_y + 0.5*d_y;
        } else {
            d_x = 0;
            d_y = 0;
            goal_pos_x = a_x;
            goal_pos_y = a_y;
        }
    } else {
        a_x = 0;
        a_y = 0;
        if(dalive) {
            d_x = 78.5-sin(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            d_y = 178+cos(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            goal_pos_x = d_x;
            goal_pos_y = d_y;
        } else {
            d_x = 0;
            d_y = 0;
        }
    }
    if(goal_pos_x != -1000) {
        g_position = fit_to_grid(goal_pos_x, goal_pos_y);
    }
}


/*! @brief Determines the position of the ball based on the robot. */
void Position::ball_position(float orientation, double straight, double balldist) {
    if(straight != -1) {
        ball_pos_x = pos_x + sin(floatMod(straight + orientation, 360)*M_PI/180)*balldist;
        ball_pos_y = pos_y - cos(floatMod(straight + orientation, 360)*M_PI/180)*balldist;
    }
}


/*! @brief Converts x/y coordinates to the 6x8 grid. */
int Position::fit_to_grid(float x, float y) {
    return 10*int(x/27)+int(y/27);
}


/*! @brief Checks that the sideways lrfs have the right distances. */
bool Position::check_x() {
    if(l_l != -1000) {
        return abs(l_l - r_l) < 400;
    }
    return false;
}


/*! @brief Checks that the front and backwards lrfs have the right distances. */
bool Position::check_y() {
    if(b_l != -1000) {
        return abs(b_l - f_l) < 400;
    }
    return false;
}