/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/
#pragma once

#include "factory.h"

// Physics Engine Calls
void physicsMovements(float delta);
void physicsCollisions();

// Collision Detection
bool isColliding(PositionVector position1, PositionVector position2, float collisionDistance);
void asteroidCollisions();

// Asteroid Physics
void explodeAsteroid(int index);
void splitAsteroid(Asteroid* asteroid);