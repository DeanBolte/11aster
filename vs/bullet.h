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

	// Update functions
	void move(float delta);

public:
	// Constructors
	Bullet(PositionVector position, PositionVector moveVector);

	// Update calls
	void update(float delta);

	// Access
	PositionVector getPosition();
	PositionVector getMoveVector();
};