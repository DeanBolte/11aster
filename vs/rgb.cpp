/*
*	the rgb class stores colour data
*
*	Main Author: Dean
*/

#include "rgb.h"

RGB::RGB(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

// Access
float RGB::getRed() {
	return r;
}

float RGB::getGreen() {
	return g;
}

float RGB::getBlue() {
	return b;
}