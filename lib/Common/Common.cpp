#include <Common.h>

double radiansToDegrees(double radians) {
	return radians*TO_DEGREES;
}

double degreesToRandians(double degrees) {
    return degrees*TO_RADIANS;
}

float floatMod(float x, float m) {
	float r = fmod(x, m);
	return r < 0 ? r + m : r;
}