#include <Lrf.h>
#include <Wire.h>
#include <VL53L1X.h>


const int sensorCount = 4;
VL53L1X sensors[sensorCount];


/*! @brief Initialises each laser range finder. Based off sample code.*/
void LRF::init() {
    toothSerial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    // Disable/reset all sensors by driving their XSHUT pins low.
    for (int i = 0; i < sensorCount; i++) {
        pinMode(lrf_pins[i], OUTPUT);
        digitalWrite(lrf_pins[i], LOW);
    }

    // Enable, initialize, and start each sensor, one by one.
    for (int i = 1; i < sensorCount; i++) {
        // Stop driving this sensor's XSHUT low. This should allow the carrier
        // board to pull it high. (We do NOT want to drive XSHUT high since it is
        // not level shifted.) Then wait a bit for the sensor to start up.
        pinMode(lrf_pins[i], INPUT);
        delay(10);

        sensors[i].setTimeout(500);
        if (!sensors[i].init())
        {
            lrf_init[i] = false;
            // Serial.print("Failed to detect and initialize sensor ");
            // Serial.println(i);
        }

        // Each sensor must have its address changed to a unique value other than
        // the default of 0x29 (except for the last one, which could be left at
        // the default). To make it simple, we'll just count up from 0x2A.
        sensors[i].setAddress(0x2A + i);

        sensors[i].startContinuous(50);
    }
}


/*! @brief Reads a single LRF.*/
void LRF::read_one(int lrf_no) {
    lrf_values[lrf_no] = sensors[lrf_no].read();
}


/*! @brief Loops through and reads every LRF.*/
void LRF::read_all() {
    lrf_values[0] = 0;
    lrf_values[1] = 0;
    lrf_values[2] = 0;
    lrf_values[3] = 0;
    for(int i = 0; i < 4; i++) {
        if(lrf_init[i]) {
            read_one(i);
        }
    }
}