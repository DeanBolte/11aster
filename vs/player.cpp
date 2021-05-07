/*
*	the player class represents the data for the player
*
*	Main Author: Dean
*/

#include "player.h"

Player::Player(float x, float y) {
	this->position.x = x;
	this->position.y = y;
	this->moveVector.x = 0;
	this->moveVector.y = 0;
	this->direction.x = 0.866;
	this->direction.y = 0.5;
	this->hp = 5;
	this->maxHp = 5;
	// Cooldowns in seconds
	this->cannonCoolDown = 0;
	this->particleCoolDown = 0;
	this->collisionRadius = PLAYER_COLLISION_RADIUS;
	this->acceleration = PLAYER_ACCELERATION;
	this->maxVelocity = PLAYER_MAXIMUM_VELOCITY;
	this->minVelocity = PLAYER_MINIMUM_VELOCITY;
}

// Access
PositionVector Player::getPosition() {
	return position;
}

PositionVector Player::getMoveVector() {
	return moveVector;
}

PositionVector Player::getDirection() {
	return direction;
}

int Player::getHp() {
	return hp;
}

int Player::getMaxHp() {
	return maxHp;
}

float Player::getParticleCoolDown() {
	return particleCoolDown;
}

float Player::getCannonCoolDown() {
	return cannonCoolDown;
}

float Player::getCollisionRadius() {
	return collisionRadius;
}

float Player::getAcceleration() {
	return acceleration;
}

float Player::getMaxVelocity() {
	return maxVelocity;
}

float Player::GetMinVelocity() {
	return minVelocity;
}