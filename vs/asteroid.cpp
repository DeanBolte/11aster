/*
*	the asteroid class represents the data for the asteroid
*
*	Main Author: Dean
*/

#include "asteroid.h"

Asteroid::Asteroid(float x, float y) {
	// Body
	position.x = x;
	position.y = y;
	moveVector.x = 0;
	moveVector.y = 0;
	
	// Vertices to draw
	vertexCount = ASTEROID_VERTEX_COUNT;
	
	// Stats
	size = rand() % 2 + 1;
	hp = size * ASTEROID_HP_MULTIPLIER;
	angle = PI / 2;
	spin = rand() % ASTEROID_SPIN - ASTEROID_SPIN / 2;

	// Collision
	collisionRadius = size + ASTEROID_BASE_SIZE + ASTEROID_VARIANCE * size / 2;

	PositionVector vertex = PositionVector(
		rand() % ASTEROID_VARIANCE - ASTEROID_VARIANCE / 2, 
		rand() % ASTEROID_VARIANCE + ASTEROID_BASE_SIZE);
	for (int i = 0; i < vertexCount; ++i) {
		vertices.push_back(vertex);
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

		vertex = rotateVector(vertex, (2 * PI) / vertexCount, 1);
	}
}

Asteroid::Asteroid(PositionVector position) : Asteroid(position.x, position.y) {}

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
PositionVector Asteroid::getVertex(int index) {
	return vertices.at(index);
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