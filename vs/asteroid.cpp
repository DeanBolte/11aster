/*
*	the asteroid class represents the data for the asteroid
*
*	Main Author: Dean
*/

#include "asteroid.h"

Asteroid::Asteroid(float x, float y) {
	// Body
	this->position.x = x;
	this->position.y = y;
	this->moveVector.x = 0;
	this->moveVector.y = 0;
	
	// Vertices to draw
	this->vertexCount = ASTEROID_VERTEX_COUNT;
	
	// Stats
	this->size = rand() % 2 + 1;
	this->hp = this->size * ASTEROID_HP_MULTIPLIER;
	this->angle = PI / 2;
	this->spin = rand() % ASTEROID_SPIN - ASTEROID_SPIN / 2;

	// Collision
	this->collisionRadius = this->size + ASTEROID_BASE_SIZE + ASTEROID_VARIANCE * this->size / 2;

	// Create vertices
	this->vertices[this->vertexCount];

	PositionVector vertex;
	vertex.x = rand() % ASTEROID_VARIANCE - ASTEROID_VARIANCE / 2;
	vertex.y = rand() % ASTEROID_VARIANCE + ASTEROID_BASE_SIZE;
	for (int i = 0; i < this->vertexCount; ++i) {
		this->vertices[i] = vertex;
		vertex.x += rand() % ASTEROID_VARIANCE - ASTEROID_VARIANCE / 4;
		vertex.y += rand() % ASTEROID_VARIANCE - ASTEROID_VARIANCE / 4;
		// check for variance limit to prevent oversized or undersized asteroids
		if (vectorLength(vertex) > ASTEROID_BASE_SIZE + MAX_ASTEROID_VARIANCE) {
			float vectorScaler = (ASTEROID_BASE_SIZE + MAX_ASTEROID_VARIANCE) / vectorLength(vertex);
			vertex = multiplyVector(vertex, vectorScaler);
		}
		else if (vectorLength(vertex) < ASTEROID_BASE_SIZE - 1.5 * MAX_ASTEROID_VARIANCE) {
			float vectorScaler = (ASTEROID_BASE_SIZE - MAX_ASTEROID_VARIANCE) / vectorLength(vertex);
			vertex = multiplyVector(vertex, vectorScaler);
		}

		vertex = rotateVector(vertex, (2 * PI) / this->vertexCount, 1);
	}
}

// Update calls
void Asteroid::update(float delta) {
	// Update position
	move(delta);

	// Spin
	angle += spin * delta;
}

void Asteroid::move(float delta) {
	position = addVectors(position, multiplyVector(moveVector, delta));
}

// Movement
PositionVector Asteroid::getPosition() {
	return position;
}

void Asteroid::setPosition(PositionVector position) {
	this->position = position;
}

PositionVector Asteroid::getMoveVector() {
	return moveVector;
}

void Asteroid::setMoveVector(PositionVector moveVector) {
	this->moveVector = moveVector;
}

float Asteroid::getCollisionRadius() {
	return collisionRadius;
}

float Asteroid::getSpin() {
	return spin;
}

// Rendering
// This functions returns an editable vertex
PositionVector* Asteroid::getVertex(int vertex) {
	PositionVector* v = NULL;
	if (vertex < vertexCount && vertex >= 0) {
		v = &vertices[vertex];
	}
	return v;
}

int Asteroid::getVertexCount() {
	return vertexCount;
}

int Asteroid::getSize() {
	return size;
}

void Asteroid::setSize(int size) {
	this->size = size;
}

float Asteroid::getAngle() {
	return angle;
}

// Health
int Asteroid::getHp() {
	return hp;
}

void Asteroid::incrementHp(int increment) {
	hp += increment;
}