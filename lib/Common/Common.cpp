#include <Common.h>


/*! @brief Converts an angle in radians to degrees. */
double radiansToDegrees(double radians) {
	return radians*TO_DEGREES;
}


/*! @brief Converts an angle in degrees to radians. */
double degreesToRandians(double degrees) {
  return degrees*TO_RADIANS;
}


/*! @brief Finds the remainder of a float. */
float floatMod(float x, float m) {
	float r = fmod(x, m);
	return r < 0 ? r + m : r;
}


/*! @brief Rounds a float to a specified number of decimal places. */
float roundtodecimal(float value, unsigned char prec) {
  float pow_10 = pow(10.0f, (float)prec);
  return round(value * pow_10) / pow_10;
}