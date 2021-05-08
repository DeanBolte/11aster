/*
*	the particle class represents the data for various particles
*
*	Main Author: Dean
*/

#include "particle.h"

// Constructors
Particle::Particle(PositionVector position, PositionVector direction) {
	// positon = player position + randomness - player direction
	// player direction is subtracted to spawn particles at the back of the ship
	this->position.x = position.x + (rand() % 10 - 5) - direction.x * 10;
	this->position.y = position.y + (rand() % 10 - 5) - direction.y * 10;

	// velocity is based on the inverse direction of acceleration of the player + some randomness
	moveVector.x = -direction.x * 1000 + rand() % 200;
	moveVector.y = -direction.y * 1000 + rand() % 200;
	decaySpeed = rand() % (PARTICLE_DECAY_SPEED / 2) + PARTICLE_DECAY_SPEED;
	size = rand() % MAX_INITIAL_SIZE;
	spin = rand() % 5 - 2.5;
	angle = PI / 2;
}

// Update functions
void Particle::move(float delta) {
	position = addVectors(position, multiplyVector(moveVector, delta));
}

// Update calls
void Particle::update(float delta) {
	// Update position
	move(delta);

	// Spin particle
	angle += spin * delta;
	if (angle >= 2 * PI) {
		angle -= 2 * PI;
	}
	else if (angle <= 0) {
		angle += 2 * PI;
	}
}

void Particle::decay(float delta) {
	size -= decaySpeed * delta;
}

// Movement
PositionVector Particle::getPosition() {
	return position;
}

PositionVector Particle::getMoveVector() {
	return moveVector;
}

// Render
float Particle::getSize() {
	return size;
}

float Particle::getAngle() {
	return angle;
}