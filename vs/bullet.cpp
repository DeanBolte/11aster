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

// Update calls
void Bullet::update(float delta) {
	// Update position
	move(delta);
}

void Bullet::move(float delta) {
	position = addVectors(position, multiplyVector(moveVector, delta));
}

// Access
PositionVector Bullet::getPosition() {
	return position;
}

PositionVector Bullet::getMoveVector() {
	return moveVector;
}