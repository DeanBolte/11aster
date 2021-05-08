/*
*	the bullet class represents the data for the bullet
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

class Bullet {
private:
	// Body
	PositionVector position;
	PositionVector moveVector;

public:
	// Constructors
	Bullet(PositionVector position, PositionVector moveVector);

	// Access
	PositionVector getPosition();
	PositionVector getMoveVector();
};