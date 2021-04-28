/*
*	source file for utility functions
*
*	Author: Dean
*	Copyright: read description in main.c
*/
#include "util.h"

// Vector math
PositionVector addVectors(PositionVector vector1, PositionVector vector2) {
	vector1.x += vector2.x;
	vector1.y += vector2.y;

	return vector1;
}

PositionVector subtractVectors(PositionVector vector1, PositionVector vector2) {
	vector1.x -= vector2.x;
	vector1.y -= vector2.y;

	return vector1;
}

PositionVector rotateVector(PositionVector vector, float radians, int dir) {
	float x1 = vector.x;
	float y1 = vector.y;
	vector.x = (x1 * cos(radians * dir)) - (y1 * sin(radians * dir));
	vector.y = (x1 * sin(radians * dir)) + (y1 * cos(radians * dir));
	return vector;
}

PositionVector multiplyVector(PositionVector vector, float multiplier) {
	vector.x *= multiplier;
	vector.y *= multiplier;
	return vector;
}

float vectorLength(PositionVector vector) {
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

float vectorAngle(PositionVector vector) {
	float radians = asin(vector.y / vectorLength(vector));
	if (vector.x < 0) {
		radians = PI - radians;
	}
	return radians;
}

PositionVector angleToUnitVector(float radians) {
	PositionVector vector;
	vector.x = cos(radians);
	vector.y = sin(radians);
	return vector;
}

PositionVector vectorToUnitVector(PositionVector vector) {
	return multiplyVector(vector, 1 / vectorLength(vector));
}

// Other Math
int intToCharacterCount(int number) {
	int numOfDigits = 1;
	if (number != 0) {
		numOfDigits = log10(number) + 1;
	}
	return numOfDigits;
}