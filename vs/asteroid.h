/*
*	the asteroid class represents the data for the asteroid
*
*	Main Author: Dean
*/
#pragma once

#include <vector>
#include "util.h"

#define ASTEROID_SPIN 6
#define ASTEROID_VERTEX_COUNT 8
#define ASTEROID_BASE_SIZE 16
#define ASTEROID_VARIANCE 4
#define MAX_ASTEROID_VARIANCE 4
#define ASTEROID_HP_MULTIPLIER 3;

#define ASTEROID_PUFF_SIZE 20
#define ASTEROID_SPLIT_RADIUS_MULTIPLIER 5
#define ASTEROID_SPLIT_ANGLE PI / 6
#define ASTEROID_PUFF_COUNT 50

class Asteroid {
private:
	PositionVector position;
	PositionVector moveVector;
	std::vector<PositionVector> vertices;
	float collisionRadius;
	int vertexCount;
	int size;
	int hp;
	float spin;
	float angle;

	// Update functions
	void move(float delta);

public:
	// Constructors
	Asteroid(float x, float y);
	Asteroid(PositionVector position);

	// Update calls
	void update(float delta);
	
	// Movement
	PositionVector getPosition();
	void setPosition(PositionVector position);
	PositionVector getMoveVector();
	void setMoveVector(PositionVector moveVector);
	float getCollisionRadius();
	float getSpin();

	// Rendering
	PositionVector getVertex(int index);
	int getVertexCount();
	int getSize();
	void setSize(int size);
	float getAngle();

	// Health
	int getHp();
	void incrementHp(int increment);

};