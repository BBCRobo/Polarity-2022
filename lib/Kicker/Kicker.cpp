#include <Kicker.h>


void Kicker::init() {
    pinMode(KICKER_PIN, OUTPUT);
    pinMode(LDR_PIN, INPUT);
    for(int i = 0; i < 10; i++) {
        LDR_high += analogRead(LDR_PIN);
    }
    LDR_high /= 10;
    LDR_high -= 50;    
    digitalWrite(KICKER_PIN, LOW);
}


void Kicker::fire() {
    if(check_LDR()) {
        digitalWrite(KICKER_PIN, HIGH);
        delay(35);
        digitalWrite(KICKER_PIN, LOW);
        delay(500);
    }
}


bool Kicker::check_LDR() {
    int reading = analogRead(LDR_PIN);
    if(reading <= LDR_high) {
        return true;
    }
    return false;
}