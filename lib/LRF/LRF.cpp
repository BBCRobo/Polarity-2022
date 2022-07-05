#include <Lrf.h>
#include <VL53L1X.h>


VL53L1X sensor;


/*! @brief Needs Updating*/
// void LRF::init() {
//     pinMode(LRF_MUX_ENABLE, OUTPUT);
//     digitalWrite(LRF_MUX_ENABLE, HIGH);
//     LRFMux.begin(MUX_ADDR, LRF_MUX);
//     for(int i = 0; i < 4; i++) {
//         LRFMux.setPort(i);
//         sensor.init();
//         delay(20);
//         sensor.setTimeout(250);
//         sensor.setDistanceMode(VL53L1X::DistanceMode::Short);
//         sensor.startContinuous(33);
//         delay(20);
//     }
// }


void LRF::enable(int lrf_no) {
    for(int i = 0; i < 4; i++) {
        if(i == lrf_no) {
            digitalWrite(lrf_pins[i], HIGH);
        } else {
            digitalWrite(lrf_pins[i], LOW);
        }
    }
}


void LRF::init() {
    pinMode(LRF_1, OUTPUT);
    pinMode(LRF_2, OUTPUT);
    pinMode(LRF_3, OUTPUT);
    pinMode(LRF_4, OUTPUT);
    for(int i = 0; i < 4; i++) {
        enable(i);
        sensor.init();
        delay(20);
        sensor.setTimeout(250);
        sensor.setDistanceMode(VL53L1X::DistanceMode::Medium);
        sensor.startContinuous(33);
        delay(20);
    }
}


// /*! @brief Needs Updating*/
// double LRF::read(int lrf) {
//     LRFMux.setPort(lrf);
//     return sensor.readRangeContinuousMillimeters();
// }


double LRF::read(int lrf_no) {
    enable(lrf_no);
    return sensor.readRangeContinuousMillimeters();
}