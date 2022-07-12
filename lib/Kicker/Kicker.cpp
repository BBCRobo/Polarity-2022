#include <Kicker.h>


void Kicker::init() {
    pinMode(KICKER_PIN, OUTPUT);
    pinMode(LDR_PIN, INPUT);
    for(int i = 0; i < 10; i++) {
        LDR_high += analogRead(LDR_PIN);
    }
    LDR_high /= 10;
    LDR_high -= 50;
    last_kick = micros();
    digitalWrite(KICKER_PIN, LOW);
}


void Kicker::fire() {
    unsigned long current_time = micros();
    if(kicking && current_time - last_kick > 35000) {
        kicking = false;
        digitalWrite(KICKER_PIN, LOW);
        last_kick = micros();
    }
    if(check_LDR() && !kicking && current_time - last_kick > 500000) {
        digitalWrite(KICKER_PIN, HIGH);
        kicking = true;
    }
}


bool Kicker::check_LDR() {
    int reading = analogRead(LDR_PIN);
    if(reading <= LDR_high) {
        return true;
    }
    return false;
}