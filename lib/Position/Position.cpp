#include <Position.h>


LRF laser;


void Position::init() {
    laser.init();
}


void Position::read_lrfs() {
    laser.read_all();
}


void Position::update(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, double straight, double balldist, bool aalive, bool dalive) {
    lrf_distances(orientation);
    goal_position(orientation, attackangle, attackdistance, defendangle, defenddistance, aalive, dalive);
    if(g_position - g_position%10 < 2) {
        pos_x = l_l == -1000 ? goal_pos_x : l_l;
    } else if(g_position - g_position%10 > 3) {
        pos_x = r_l == -1000 ? goal_pos_x : r_l;
    } else if(check_x()) {
        pos_x = 0.5*r_l + 0.5*l_l;
    } else {
        pos_x = goal_pos_x;
    }
    if(g_position - g_position%10 < 2) {
        pos_y = f_l == -1000 ? goal_pos_y : f_l;
    } else if(g_position - g_position%10 > 5) {
        pos_y = b_l == -1000 ? goal_pos_y : b_l;
    } else if(check_y()) {
        pos_y = 0.5*b_l + 0.5*f_l;
    } else {
        pos_y = goal_pos_y;
    }
    position = 10*int(pos_x/270)+int(pos_y/270);
    ball_position(orientation, straight, balldist);
    Serial.print(orientation);
    Serial.print(" ");
    Serial.print(f_l);
    Serial.print(" ");
    Serial.print(b_l);
    Serial.print(" ");
    Serial.print(l_l);
    Serial.print(" ");
    Serial.print(r_l);
    Serial.print(" ");
    Serial.print(a_x);
    Serial.print(" ");
    Serial.print(a_y);
    Serial.print(" ");
    Serial.print(d_x);
    Serial.print(" ");
    Serial.print(d_y);
    Serial.print(" ");
    Serial.print(pos_x);
    Serial.print(" ");
    Serial.print(pos_y);
    Serial.print(" ");
    Serial.print(g_position);
    Serial.print(" ");
    Serial.println(position);
}


void Position::lrf_distances(float orientation) {
    if(abs(orientation - 360) < 45 || abs(orientation - 0) < 45) {
        if(laser.lrf_init[0]) {
            f_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[1]) {
            b_l = 2050 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[2]) {
            l_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[3]) {
            r_l = 1570 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180);
        }
    } else if(abs(orientation - 270) <= 45) {
        if(laser.lrf_init[0]) {
            l_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[1]) {
            r_l = 2050 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[2]) {
            b_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[3]) {
            f_l = 1570 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180);
        }
    } else if(abs(orientation - 180) < 45) {
        if(laser.lrf_init[0]) {
            b_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[1]) {
            f_l = 2050 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[2]) {
            r_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[3]) {
            l_l = 1570 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180);
        }
    } else {
        if(laser.lrf_init[0]) {
            r_l = laser.lrf_values[0]*cos(floatMod(orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[1]) {
            l_l = 2050 + laser.lrf_values[1]*cos(floatMod(180 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[2]) {
            f_l = -laser.lrf_values[2]*sin(floatMod(270 + orientation, 360)*M_PI/180);
        }
        if(laser.lrf_init[3]) {
            b_l = 1570 - laser.lrf_values[3]*sin(floatMod(90 + orientation, 360)*M_PI/180);
        }
    }
}


void Position::goal_position(float orientation, double attackangle, double attackdistance, double defendangle, double defenddistance, bool aalive, bool dalive) {
    if(aalive) {
        a_x = 785-10*sin(floatMod(attackangle + orientation, 360)*M_PI/180)*attackdistance;
        a_y = 270+10*cos(floatMod(attackangle + orientation, 360)*M_PI/180)*attackdistance;
        if(dalive) {
            d_x = 785-10*sin(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            d_y = 1780+10*cos(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            goal_pos_x = 0.5*a_x + 0.5*d_x;
            goal_pos_y = 0.5*a_y + 0.5*d_y;
        } else {
            goal_pos_x = a_x;
            goal_pos_y = a_y;
        }
    } else {
        if(dalive) {
            d_x = 785-10*sin(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            d_y = 1780+10*cos(floatMod(defendangle + orientation, 360)*M_PI/180)*defenddistance;
            goal_pos_x = d_x;
            goal_pos_y = d_y;
        }
    }
    if(goal_pos_x != -1000) {
        g_position = fit_to_grid(goal_pos_x, goal_pos_y);
    }
}


void Position::ball_position(float orientation, double straight, double balldist) {
    if(straight != -1) {
        ball_pos_x = pos_x + sin(floatMod(straight + orientation, 360)*M_PI/180)*balldist;
        ball_pos_y = pos_y + cos(floatMod(straight + orientation, 360)*M_PI/180)*balldist;
    }
}


int Position::fit_to_grid(float x, float y) {
    return 10*int(x/270)+int(y/270);
}


bool Position::check_x() {
    if(l_l != -1000) {
        return abs(l_l - r_l) < 400;
    }
    return false;
}


bool Position::check_y() {
    if(b_l != -1000) {
        return abs(b_l - f_l) < 400;
    }
    return false;
}