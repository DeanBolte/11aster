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
void asteroidCollisions();
void blackHoleCollision();
int collidingWithBlackHole(PositionVector position, float radius);

// Asteroid Physics
void explodeAsteroid(int index);
void splitAsteroid(Asteroid* asteroid);

// Movement
// Movement calls
PositionVector movePosition(float delta, PositionVector position, PositionVector movement);

// Asteroid Movement
void moveAsteroid(float delta, Asteroid* asteroid);

// Player Movement
void movePlayer(float delta);

// Bullet Movement
void moveBullets(float delta);

// Particle Movement
void moveParticles(float delta);

// Black Hole
void applyGravity(float delta);
PositionVector getAccelerationVector(float delta, BlackHole* bh, PositionVector objectPosition);