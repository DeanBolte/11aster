/*
*	the particle class represents the data for various particles
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

#define PARTICLE_DECAY_SPEED 30
#define MAX_INITIAL_SIZE 10

class Particle {
private:
	// Body
	PositionVector position;
	PositionVector moveVector;
	float decaySpeed;
	float spin;
	float angle;
	float size;

	// Update functions
	void move(float delta);

public:
	// Constructors
	Particle(PositionVector position, PositionVector direction);

	// Update calls
	void update(float delta);
	void decay(float delta);

	// Movement
	PositionVector getPosition();
	PositionVector getMoveVector();

	// Render
	float getSize();
	float getAngle();

};