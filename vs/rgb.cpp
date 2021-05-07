/*
*	the rgb class stores colour data
*
*	Main Author: Dean
*/

#include "rgb.h"

Colour::Colour(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

// Access
float Colour::getRed() {
	return r;
}

float Colour::getGreen() {
	return g;
}

float Colour::getBlue() {
	return b;
}