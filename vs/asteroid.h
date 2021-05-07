/*
*	the asteroid class represents the data for the asteroid
*
*	Main Author: Dean
*/
#pragma once

#include "util.h"

#define ASTEROID_SPIN 6
#define ASTEROID_VERTEX_COUNT 8
#define ASTEROID_BASE_SIZE 16
#define ASTEROID_VARIANCE 4
#define MAX_ASTEROID_VARIANCE 4
#define ASTEROID_HP_MULTIPLIER 3;

class Asteroid {
private:
	PositionVector position;
	PositionVector moveVector;
	PositionVector* vertices;
	int inside;
	float collisionRadius;
	int vertexCount;
	int size;
	int hp;
	float spin;
	float angle;

public:
	Asteroid(float x, float y);

};