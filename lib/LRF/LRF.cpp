#include <Lrf.h>

QWIICMUX LRFMUX;
VL53L1X sensor;

void Lrf::init() {
    pinMode(LRF_MUX_ENABLE, OUTPUT);
    digitalWrite(LRF_MUX_ENABLE, HIGH);
    LRFMux.begin(MUX_ADDR, LRF_MUX);
    for(int i = 0; i < 4; i++) {
        LRFMux.setPort(i);
        sensor.init();
        delay(20);
        sensor.setTimeout(250);
        sensor.setDistanceMode(VL53L1X::DistanceMode::Short);
        sensor.startContinuous(33);
        delay(20);
    }
}

double Lrf::read(int lrf) {
    LRFMux.setPort(lrf);
    return sensor.readRangeContinuousMillimeters();
}