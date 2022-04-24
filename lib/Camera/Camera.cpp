#include <Camera.h>

void Camera::init() {
    cameraSerial.begin(CAM_BAUD);
    count = 0;
    achange = 0;
    totalvariance = 0;
}

void Camera::read() {
    while(cameraSerial.available() >= CAM_PACKET_SIZE) {
        if(cameraSerial.read() == CAM_START_BYTE) {
            for(int i = 0; i < CAM_PACKET_SIZE - 1; i++) {
                camBuffer[i] = cameraSerial.read();
            }
            aalive = camBuffer[0] != CAM_NO_DATA;
            dalive = camBuffer[2] != CAM_NO_DATA;
            balive = camBuffer[4] != CAM_NO_DATA;
            attackangle = aalive ? calcangle(camBuffer[0], camBuffer[1]) : -1;
            attackdistance = calcdistance(camBuffer[0], camBuffer[1]);
            defendangle = dalive ? calcangle(camBuffer[2], camBuffer[3]) : -1;
            defenddistance = calcdistance(camBuffer[2], camBuffer[3]);
            straight = balive ? calcangle(camBuffer[4], camBuffer[5]) : -1;
            balldist = calcdistance(camBuffer[4], camBuffer[5]);
            // experimental[int(straight)] += balldist;
            // experimental[int(straight)] /= 2;
            // Serial.print(straight);
            // Serial.print("  ");
            // Serial.println(attackdistance);
            // Serial.print(experimental[300]);
            // Serial.print("  ");
            // Serial.print(experimental[310]);
            // Serial.print("  ");
            // Serial.print(experimental[320]);
            // Serial.print("  ");
            // Serial.print(experimental[330]);
            // Serial.print("  ");
            // Serial.print(experimental[340]);
            // Serial.print("  ");
            // Serial.println(experimental[350]);
            // for(int j = 0; j == 360; j++) {
            //     Serial.print(experimental[j]);
            //     Serial.print(",");
            // }
            // Serial.println("");
        }
    }
}

double Camera::calcangle(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    return floatMod(270 - round(radiansToDegrees(atan2(y,x))), 360);
}

double Camera::calcdistance(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    int pixels = sqrt(x * x + y * y);
    return pixels;
}