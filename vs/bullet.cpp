/*
*	the bullet class represents the data for the bullet
*
*	Main Author: Dean
*/

#include "bullet.h"

// Constructors
Bullet::Bullet(PositionVector position, PositionVector moveVector) {
	this->position = position;
	this->moveVector = moveVector;
}

// Access
PositionVector Bullet::getPosition() {
	return position;
}

PositionVector Bullet::getMoveVector() {
	return moveVector;
}