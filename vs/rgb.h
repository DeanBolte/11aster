/*
*	the rgb class stores colour data
*
*	Main Author: Dean
*/
#pragma once

class Colour {
private:
	// Colour values
	float r, g, b;
public:
	// Constructors
	Colour(float r, float g, float b);

	// Access colours
	float getRed();
	float getGreen();
	float getBlue();
};