/*
*	the player class represents the data for the player
*
*	Main Author: Dean
*/

#include "player.h"

// Constructors
Player::Player(float x, float y) {
	// Body
	position = PositionVector(x, y);
	moveVector = PositionVector();
	direction = PositionVector(0.866, 0.5);

	// Collision
	collisionRadius = PLAYER_COLLISION_RADIUS;

	// Health
	hp = 5;
	maxHp = 5;

	// Cooldowns in seconds
	cannonCoolDown = 0;
	particleCoolDown = 0;

	// Movement
	acceleration = PLAYER_ACCELERATION;
	maxVelocity = PLAYER_MAXIMUM_VELOCITY;
	minVelocity = PLAYER_MINIMUM_VELOCITY;
}

Player::Player(PositionVector position) : Player(position.x, position.y) {}

// Update calls
void Player::update(float delta) {
	// Update position
	move(delta);
}

// Player Movement
void Player::move(float delta) {
	position = addVectors(position, multiplyVector(moveVector, delta));
}

// Movement
PositionVector Player::getPosition() {
	return position;
}

PositionVector Player::getMoveVector() {
	return moveVector;
}

PositionVector Player::getDirection() {
	return direction;
}

void Player::rotatePlayer(float degree) {
	direction = rotateVector(direction, degree, 1);
}

void Player::acceleratePlayer(float delta, int dir) {
	// Accelerate player velocity towards player direction
	// Only accelerate when below max velocity
	if (dir > 0 && vectorLength(moveVector) <= maxVelocity) {
		PositionVector accelerate = multiplyVector(direction, acceleration * dir * delta);
		PositionVector newMoveVector = addVectors(moveVector, accelerate);

		// slow ship to max velocity
		if (vectorLength(newMoveVector) < maxVelocity) {
			moveVector = newMoveVector;
		}
	}
}

void Player::brakePlayer(float delta) {
	if (vectorLength(moveVector) > PLAYER_MINIMUM_VELOCITY) {
		PositionVector accelerate = multiplyVector(moveVector, PLAYER_BRAKE_MULTIPLIER * delta);
		moveVector = subtractVectors(moveVector, accelerate);
	}
	else {
		moveVector = multiplyVector(moveVector, 0);
	}
}

// Health
int Player::getHp() {
	return hp;
}

void Player::incrementHp(int increment) {
	hp += increment;
}

int Player::getMaxHp() {
	return maxHp;
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

// Cooldowns
float Player::getParticleCoolDown() {
	return particleCoolDown;
}

void Player::incrementParticleCoolDown(float increment) {
	particleCoolDown += increment;
}

float Player::getCannonCoolDown() {
	return cannonCoolDown;
}

void Player::incrementCannonCoolDown(float increment) {
	cannonCoolDown += increment;
}
