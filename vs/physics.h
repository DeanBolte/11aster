/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/
#pragma once

#include "objects.h"

// Physics Engine Calls
void physicsMovements(float delta);
void physicsCollisions();

// Collision Detection
bool isColliding(PositionVector position1, PositionVector position2, float collisionDistance);
void asteroidCollisions();

// Asteroid Physics
void explodeAsteroid(int index);
void splitAsteroid(Asteroid* asteroid);

// Movement
// Movement calls
PositionVector movePosition(float delta, PositionVector position, PositionVector movement);

// Particle Movement
void moveParticles(float delta);