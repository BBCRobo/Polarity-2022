#include <Camera.h>


/*! @brief Initialises the camera. */
void Camera::init() {
    cameraSerial.begin(CAM_BAUD);
}


/*! @brief The read function will retrieve any data written to the teensy through UART. It then calculates the angle and distances from the goals and the ball. */
void Camera::read(float heading) {
    // Serial.println(cameraSerial.available());
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
            int attackpixels = camBuffer[6] * 100 + camBuffer[7];
            int attacktrueangle = int(attackangle+heading) % 360;
            defendangle = dalive ? calcangle(camBuffer[2], camBuffer[3]) : -1;
            defenddistance = dalive ? calcdistance(camBuffer[2], camBuffer[3]) : -1;
            int defendpixels = camBuffer[8]*100 + camBuffer[9];
            int defendtrueangle = int(defendangle+heading) % 360;
            if(ROBOT == 0) {
                //Centre Method
                if(ATTACK_GOAL_YELLOW) {
                    attackdistance = aalive ? pow(euler, (-0.00105485*sin((M_PI/180)*(attackangle+59.8667))+0.0415387)*attackdistance)+1.76269*sin((M_PI/180)*(attackangle+73.0462))-11.0853 : -1;
                } else {
                    defenddistance = dalive ? pow(euler, (-0.00105485*sin((M_PI/180)*(defendangle+59.8667))+0.0415387)*defenddistance)+1.76269*sin((M_PI/180)*(defendangle+73.0462))-11.0853 : -1;
                }
            } else {
                //Pixel Method
                attackdistance = (-31389.5*sin((M_PI/180)*(attacktrueangle-104310))+126224)/(attackpixels-388.138*sin((M_PI/180)*(attacktrueangle-2790))+868.47)+54.2258*sin((M_PI/180)*(attacktrueangle-270))-56.0895;
                defenddistance = (31387.9*sin((M_PI/180)*(defendtrueangle+74610))+126201)/(defendpixels+388.138*sin((M_PI/180)*(defendtrueangle-630))+868.47)-54.226*sin((M_PI/180)*(defendtrueangle-630))-56.0896;
            }
            // Serial.print(attackpixels);
            // Serial.print(" ");
            // Serial.print(attackdistance);
            // Serial.print(" ");
            // Serial.print(defendangle);
            // Serial.print(" ");
            // Serial.print(defendpixels);
            // Serial.print(" ");
            // Serial.println(defenddistance);
            straight = balive ? calcangle(camBuffer[4], camBuffer[5]) : -1;
            balldist = calcdistance(camBuffer[4], camBuffer[5]);
            // balldist = 5.94947+pow(euler, (balldist-7.82656)/20.3883);
            // balldist = pow(euler, (0.00279347*sin((M_PI/180)*(straight-60.537))+0.0384934)*balldist)+2.62259*sin((M_PI/180)*(straight-9.35646))-5.47036+10;
            // balldist = pow(euler, (0.00130576*sin((M_PI/180)*(straight-88.396))+0.038705)*balldist)-0.681997*sin((M_PI/180)*(straight-124.038))-8.85295+8.5;
            balldist = pow(euler, (-0.00107068*sin((M_PI/180)*(straight-25.2786))+0.0433359)*balldist)+0.251807*sin((M_PI/180)*(straight-2.89666))-5.57559+8.5;
        }
    }
}


/*! @brief Calculates the angle of the blob's centre with relation to the centre of the robot. 0 degrees is forward. */
double Camera::calcangle(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    return floatMod(90 - round(radiansToDegrees(atan2(y,x))), 360);
}


/*! @brief Calculates the pixel distance of the blob's centre with relation to the centre of the robot. */
double Camera::calcdistance(int _x, int _y) {
    int x = _x - (CAM_CENTRE_X);
    int y = _y - (CAM_CENTRE_Y);
    int pixels = sqrt(x * x + y * y);
    return pixels;
}