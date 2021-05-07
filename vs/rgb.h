/*
*	the rgb class stores colour data
*
*	Main Author: Dean
*/
#pragma once

class RGB {
private:
	// Colour values
	float r, g, b;
public:
	// Constructors
	RGB(float r, float g, float b);

	// Access
	float getRed();
	float getGreen();
	float getBlue();
};