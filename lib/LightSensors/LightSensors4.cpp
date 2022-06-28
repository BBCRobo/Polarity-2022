#include <LightSensors4.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Common.h>
#include <math.h>

int LightSensor4::read(int no) {
    int multiplexerpins[4] = {LS_BINARY_0, LS_BINARY_1, LS_BINARY_2, LS_BINARY_3};
    if(no >= 16) {
        no -= 16;
        int power = 8;
        for(int i = 3; i >= 0; i--) {
            if(no >= power) {
                digitalWrite(multiplexerpins[i], HIGH);
                no -= power;
            } else {
                digitalWrite(multiplexerpins[i], LOW);
            }
            power = int(power/2);
        }
        return analogRead(LS_OUTPUT_2);
    } else {
        int power = 8;
        for(int i = 3; i >= 0; i--) {
            if(no >= power) {
                digitalWrite(multiplexerpins[i], HIGH);
                no -= power;
            } else {
                digitalWrite(multiplexerpins[i], LOW);
            }
            power = int(power/2);
        }
        return analogRead(LS_OUTPUT_1);
    }
}

void LightSensor4::init() {
    pinMode(LS_BINARY_0, OUTPUT);
    pinMode(LS_BINARY_1, OUTPUT);
    pinMode(LS_BINARY_2, OUTPUT);
    pinMode(LS_BINARY_3, OUTPUT);
    pinMode(LS_OUTPUT_1, INPUT);
    pinMode(LS_OUTPUT_2, INPUT);
    for(int i = 0; i < LS_COUNT; i++) {
        // green[i] = read(lspins[i]);
        green[i] = EEPROM.read(i)*4;
    }
    location = 0;
    prev = -11.25;
}

void LightSensor4::FindGreen() {
    unsigned long start_time = millis();
    for(int i = 0; i < LS_COUNT; i++) {
        lsdebug[i][0] = read(lspins[i]);
    }
    unsigned long end_time = millis();
    while(end_time-start_time < 5000) {
        for(int i = 0; i < LS_COUNT; i++) {
            int value = read(lspins[i]);
            if(value > lsdebug[i][0]) {
                lsdebug[i][0] = value;
            }
        }
        end_time = millis();
    }
}

void LightSensor4::FindWhite() {
    unsigned long start_time = millis();
    for(int i = 0; i < LS_COUNT; i++) {
        lsdebug[i][1] = read(lspins[i]);
    }
    unsigned long end_time = millis();
    while(end_time-start_time < 5000) {
        for(int i = 0; i < LS_COUNT; i++) {
            int value = read(lspins[i]);
            if(value > lsdebug[i][1]) {
                lsdebug[i][1] = value;
            }
        }
        end_time = millis();
    }
}

double LightSensor4::AngleBetweenClusters(float centre1, float centre2) {
    double vector1[2] = {cos(centre1*11.25*M_PI/180),sin(centre1*11.25*M_PI/180)};
    double vector2[2] = {cos(centre2*11.25*M_PI/180),sin(centre2*11.25*M_PI/180)};
    double final_vector[2] = {(vector1[0]+vector2[0])/2,(vector1[1]+vector2[1])/2};
    double final_angle = atan2(final_vector[1], final_vector[0]);
    double anglebetween = 2*min(abs(final_angle-atan2(vector1[1],vector1[0])), abs(final_angle-atan2(vector2[1],vector2[0])))*180/M_PI;
    return anglebetween;
}

double LightSensor4::AngleBetweenAngles(float angle1, float angle2) {
    double vector1[2] = {cos(angle1),sin(angle2)};
    double vector2[2] = {cos(angle1),sin(angle2)};
    double final_vector[2] = {(vector1[0]+vector2[0])/2,(vector1[1]+vector2[1])/2};
    double final_angle = atan2(final_vector[1], final_vector[0]);
    double anglebetween = 2*min(abs(final_angle-atan2(vector1[1],vector1[0])), abs(final_angle-atan2(vector2[1],vector2[0])));
    return anglebetween;
}

double LightSensor4::DirectionOfLine(float orientation, int position) {
    int lsvalues[LS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int clusters[16][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    int clusterno = -1;
    int totalno = 0;
    double expected[3] = {0,0,0};
    for(int i = 0; i < (LS_COUNT); i++) {
        // Serial.print(read(lspins[i]));
        // Serial.print(" ");
        if(int(read(lspins[i])/4)*4 > green[i]) {
            lsvalues[i] = 1;
            totalno += 1;
        } else {
            lsvalues[i] = 0;
        }
    }
    // Serial.println(";");
    clustering = false;
    for(int i = 0; i < LS_COUNT; i++) {
        Serial.print(lsvalues[i]);
        Serial.print(' ');
        if(clustering) {
            clusters[clusterno][1] = i;
            if(lsvalues[i] == 0) {
                clustering = false;
                clusters[clusterno][1] = i-1;
            }
        } else {
            if(lsvalues[i] == 1) {
                clustering = true;
                clusterno += 1;
                clusters[clusterno][0] = i;
                clusters[clusterno][1] = i;
            }
        }
    }
    Serial.println(";");
    int final_clusters[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}}; 
    clusterno = 0;
    for(int i = 0; i < 16; i++) {
        if(clusters[i][0] == -1 && clusters[i][1] == -1) {
            continue;
        }
        if(final_clusters[clusterno][0] == -1) {
            final_clusters[clusterno][0] = clusters[i][0];
            final_clusters[clusterno][1] = clusters[i][1];
        }
        if(final_clusters[clusterno][1] >= clusters[i][0] - 4) {
            final_clusters[clusterno][1] = clusters[i][1];
        } else {
            clusterno += 1;
            final_clusters[clusterno][0] = clusters[i][0];
            final_clusters[clusterno][1] = clusters[i][1];
        }
        // Serial.print("{");
        // Serial.print(clusters[i][0]);
        // Serial.print(",");
        // Serial.print(clusters[i][1]);
        // Serial.print("}");
        // Serial.print(" ");
    }
    if(final_clusters[0][0] <= final_clusters[clusterno][1]-28) {
        final_clusters[0][0] = final_clusters[clusterno][0];
        final_clusters[clusterno][0] = -1;
        final_clusters[clusterno][1] = -1;
        clusterno -= 1;
    }
    float cluster_centre[4] = {0.0,0.0,0.0,0.0};
    for(int i = 0; i < 4; i++) {
        float centre = (final_clusters[i][0] + (final_clusters[i][0] > final_clusters[i][1] ? final_clusters[i][1]+32 : final_clusters[i][1]))/2;
        centre -= centre > 31 ? 32 : 0;
        cluster_centre[i] = centre;
        // Serial.print("{");
        // Serial.print(final_clusters[i][0]);
        // Serial.print(",");
        // Serial.print(final_clusters[i][1]);
        // Serial.print("}");
        // Serial.print(" ");
    }
    clusterno += 1;
    if(totalno > 0) {
        if(clusterno == 1) {
            direction = floatMod(11.25*cluster_centre[0], 360);
        } else if(clusterno == 2) {
            double anglebetweenclusters = AngleBetweenClusters(cluster_centre[0], cluster_centre[1]);
            if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[1], 360),2)) {
                direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters/2,360),2);
            } else {
                direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+anglebetweenclusters/2,360),2);
            }
        } else {
            double anglebetweenclusters12 = roundtodecimal(AngleBetweenClusters(cluster_centre[0], cluster_centre[1]),2);
            double anglebetweenclusters13 = roundtodecimal(AngleBetweenClusters(cluster_centre[0], cluster_centre[2]),2);
            double anglebetweenclusters23 = roundtodecimal(AngleBetweenClusters(cluster_centre[1], cluster_centre[2]),2);
            if(anglebetweenclusters12 >= anglebetweenclusters13 && anglebetweenclusters12 >= anglebetweenclusters23) {
                if(anglebetweenclusters13+anglebetweenclusters23 == anglebetweenclusters12) {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters12,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[1], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters12/2,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+anglebetweenclusters12/2,360),2);
                    }
                } else {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+360-anglebetweenclusters12,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[1], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+180-anglebetweenclusters12/2+180,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+180-anglebetweenclusters12/2+180,360),2);
                    }
                }
            } else if(anglebetweenclusters13 >= anglebetweenclusters12 && anglebetweenclusters13 >= anglebetweenclusters23) {
                if(anglebetweenclusters12+anglebetweenclusters23 == anglebetweenclusters13) {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters13,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[2], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+anglebetweenclusters13/2,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[2], 360)+anglebetweenclusters13/2,360),2);
                    }
                } else {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+360-anglebetweenclusters13,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[2], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[0], 360)+180-anglebetweenclusters13/2,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[2], 360)+180-anglebetweenclusters13/2,360),2);
                    }
                }
            } else {
                if(anglebetweenclusters12+anglebetweenclusters13 == anglebetweenclusters23) {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+anglebetweenclusters23,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[2], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+anglebetweenclusters23/2,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[2], 360)+anglebetweenclusters23/2,360),2);
                    }
                } else {
                    if(roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+360-anglebetweenclusters23,360),2) == roundtodecimal(floatMod(11.25*cluster_centre[2], 360),2)) {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[1], 360)+180-anglebetweenclusters23/2,360),2);
                    } else {
                        direction = roundtodecimal(floatMod(floatMod(11.25*cluster_centre[2], 360)+180-anglebetweenclusters23/2,360),2);
                    }
                }
            }
        }
        direction *= M_PI/180;
    } else {
        direction = -11.25;
    }
    // Serial.print(floatMod(11.25*cluster_centre[0], 360));
    // Serial.print(" ");
    // Serial.print(floatMod(11.25*cluster_centre[1], 360));
    // Serial.print(" ");
    // Serial.print(floatMod(11.25*cluster_centre[2], 360));
    // Serial.print(" ");

    direction = (orientation > 180 ? orientation - 360 : orientation)*M_PI/180 + direction;
    while(direction < -M_PI) {
        direction += M_PI*2;
    }
    while(direction >= M_PI) {
        direction -= M_PI*2;
    }
    // Serial.print(direction);
    // Serial.print(" ");
    // Serial.print(prev);
    // Serial.print(" ");
    // if(position != -1) {
    //     if(position%10 < 2) {
    //         if(position-position%10 < 2) {
    //             expected[0] = M_PI/2;
    //             expected[1] = M_PI;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI;
    //             expected[1] = -M_PI/2;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = -M_PI/2;
    //             expected[1] = M_PI/2;
    //             expected[2] = 1;
    //         }
    //     } else if(position%10 > 5) {
    //         if(position-position%10 < 2) {
    //             expected[0] = 0;
    //             expected[1] = M_PI/2;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI/2;
    //             expected[1] = 0;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = -M_PI/2;
    //             expected[1] = M_PI/2;
    //             expected[2] = 0;
    //         }
    //     } else {
    //         if(position-position%10 < 2) {
    //             expected[0] = 0;
    //             expected[1] = M_PI;
    //             expected[2] = 0;
    //         } else if(position-position%10 > 3) {
    //             expected[0] = -M_PI;
    //             expected[1] = 0;
    //             expected[2] = 0;
    //         } else {
    //             expected[0] = 0;
    //             expected[1] = 0;
    //             expected[2] = -1;
    //         }
    //     }
    // } else {
    //     expected[0] = 0;
    //     expected[1] = 0;
    //     expected[2] = -2;
    // }

    if(totalno != 0) {
        if(location == 0) {
            location = 1;
            prev = direction;
        }
        if(location == 3) {
            location = 2;
        }
        if(AngleBetweenAngles(prev,direction) < 1.22) {
            if(location == 2) {
                location = 1;
            }
        } else {
            if(location == 1) {
                location = 2;
            }
        }
        // if(expected[2] == 0) {
        //     if(location == 1) {
        //         if(direction > expected[1] || direction < expected[0]) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else if(vector_modulus < 0.5) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else {
        //             prev = direction;
        //         }
        //     } else if(location == 2) {
        //         if(direction < expected[1] && direction > expected[0] && vector_modulus >= 0.5) {
        //             location = 1;
        //             prev = direction;
        //         } else {
        //             direction = prev;
        //         }
        //     }
        // } else if(expected[2] == 1) {
        //     if(location == 1) {
        //         if(direction < expected[1] && direction > expected[0]) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else if(vector_modulus < 0.5) {
        //             location = 2;
        //             direction = (expected[0]+expected[1])/2;
        //         } else {
        //             prev = direction;
        //         }
        //     } else if(location == 2) {
        //         if((direction > expected[1] || direction < expected[0]) && vector_modulus >= 0.5) {
        //             location = 1;
        //             prev = direction;
        //         } else {
        //             direction = prev;
        //         }
        //     }
        // } else if(expected[2] < 0) {
        if(location == 1) {
            prev = direction;
            direction += M_PI;
            lastseenline = millis();
        } else if(location == 2) {
            lastline = direction;
        }
        // }
    } else {
        if(location == 0) {
            location = 0;
            direction = -11.25;
            prev = -11.25;
        } else if(location == 1) {
            unsigned long now = millis();
            if(now - lastseenline > 1000) {
                location = 0;
                direction = -11.25;
                prev = -11.25;
            }
        } else if(location == 2 || location == 3) {
            // if(expected[2] == -1) {
            //     location = 0;
            //     direction = -11.25;
            //     prev = -11.25;
            // } else {
            location = 3;
            direction = lastline;
            // }
        }
    }
    if(direction != -11.25) {
        direction -= (orientation > 180 ? orientation - 360 : orientation)*M_PI/180;
        while(direction < 0) {
            direction += M_PI*2;
        }
        while(direction >= M_PI*2) {
            direction -= M_PI*2;
        }
        direction *= 180/M_PI;
    }
    // Serial.print(location);
    // Serial.print(" ");
    // Serial.println(direction);
    return direction;
}