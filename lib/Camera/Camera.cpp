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
            attackdistance = aalive ? calcdistance(camBuffer[0], camBuffer[1]) : -1;
            attackdistance = aalive ? log((attackdistance-8.79885*sin((1.00845*M_PI/180)*(attackangle-85.0709))-103.88)/(3.67491*sin((1.95003*M_PI/180)*(attackangle-26.6964))-44.0313))/(0.00319501*sin((1.03966*M_PI/180)*(attackangle-80.5454))-0.0410537) : -1;
            // attackdistance = calcdistance(camBuffer[6], camBuffer[7]);
            defendangle = dalive ? calcangle(camBuffer[2], camBuffer[3]) : -1;
            defenddistance = dalive ? calcdistance(camBuffer[2], camBuffer[3]) : -1;
            defenddistance = log((defenddistance-8.79885*sin((1.00845*M_PI/180)*(defendangle-85.0709))-103.88)/(3.67491*sin((1.95003*M_PI/180)*(defendangle-26.6964))-44.0313))/(0.00319501*sin((1.03966*M_PI/180)*(defendangle-80.5454))-0.0410537);
            straight = balive ? calcangle(camBuffer[4], camBuffer[5]) : -1;
            balldist = calcdistance(camBuffer[4], camBuffer[5]);
            balldist = 5.94947+pow(euler, (balldist-7.82656)/20.3883);
        }
    }
}

double Camera::calcangle(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    return floatMod(90 - round(radiansToDegrees(atan2(y,x))), 360);
}

double Camera::calcdistance(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    int pixels = sqrt(x * x + y * y);
    return pixels;
}