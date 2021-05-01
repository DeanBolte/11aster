/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/
#pragma once

#include "elements.h"

#define DEGREE_OF_ROTATION 2 * PI
#define PLAYER_PARTICLE_SIZE 5

#define ASTEROID_PUFF_SIZE 20
#define ASTEROID_SPLIT_RADIUS_MULTIPLIER 5
#define ASTEROID_SPLIT_ANGLE PI / 6
#define ASTEROID_PUFF_COUNT 50

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
void rotatePlayer(float delta, int dir);
void acceleratePlayer(float delta, int dir);

// Bullet Movement
void moveBullets(float delta);
void cullBullet(Bullet* bullet, int index);

// Particle Movement
void moveParticles(float delta);

// Black Hole
void applyGravity(float delta);
PositionVector getAccelerationVector(float delta, BlackHole* bh, PositionVector objectPosition);