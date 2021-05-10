/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/

#include "physics.h"

//		Physics Engine Calls
void physicsMovements(float delta) {
	// Update player
	getPlayer()->update(delta);
	
	// Update asteroid positions
	for (int i = 0; i < getAsteroidCount(); ++i) {
		getAsteroid(i)->update(delta);
	}

	// Update bullet positions
	for (int i = 0; i < getBulletCount(); ++i) {
		getBullet(i)->update(delta);
	}

	// Update particle positions
	for (int i = 0; i < getParticleCount(); ++i) {
		// Move particle
		Particle* particle = getParticle(i);
		particle->update(delta);

		// Decay and until no size then destroy
		particle->decay(delta);
		if (particle->getSize() <= 0) {
			freeParticle(i);
			// decrement i as the next particle will fill this element of the array
			--i;
		}
	}
}

void physicsCollisions() {
	// Check collision with asteroids
	// Player / Asteroid collisions
	// Bullet / Asteroid collisions
	// Asteroid / Wall collisions
	// BlackHole / Asteroid collisions
	asteroidCollisions();
}

//		Collision Detection
bool isColliding(PositionVector position1, PositionVector position2, float collisionDistance) {
	// return bool
	bool colliding = false;

	// Get distance between the two objects
	float distance = vectorLength(subtractVectors(position1, position2));

	// check if the two objects are within distance to collide
	if (distance <= collisionDistance) {
		colliding = true;
	}

	return colliding;
}

//		Asteroid stuff

// This is the heaviest part of the code base, its essentially the entire game, it could be heavily modified to be made faster
// however that is out of the scope of this project, i will however attempt to do as much optimisation in the time i have available
void asteroidCollisions() {
	for (int i = 0; i < getAsteroidCount(); ++i) {
		Asteroid* asteroid = getAsteroid(i);

		// Check for collision circle overlap for player
		Player* player = getPlayer();
		if (isColliding(player->getPosition(), asteroid->getPosition(), player->getCollisionRadius() + asteroid->getCollisionRadius())) {
			player->incrementHp(-PLAYER_DAMAGE_TAKEN);
		}

		// Check for collisions with other asteroids
		for (int j = 0; j < getAsteroidCount(); ++j) {
			if (i != j) {
				// Distance between asteroids
				PositionVector distanceVector = subtractVectors(getAsteroid(i)->getPosition(), getAsteroid(j)->getPosition());
				float distance = vectorLength(distanceVector);
				if (distance < getAsteroid(i)->getCollisionRadius() + getAsteroid(j)->getCollisionRadius()) {
					PositionVector temp;
					temp = asteroid->getMoveVector();
					asteroid->setMoveVector(getAsteroid(j)->getMoveVector());
					getAsteroid(j)->setMoveVector(temp);

					// Move the asteroids apart so they arent within each other
					// the distance of overlap between the two asteroids is equal to the distance between there centres minus the non overlapping distance
					// here i calculate the vector of overlap and divide it in two to move the asteroids apart equally
					// this is to prevent asteroid overlap
					PositionVector distanceUnitVector = vectorToUnitVector(distanceVector);
					PositionVector asteroid1OverlapVector = multiplyVector(distanceUnitVector, asteroid->getCollisionRadius());
					PositionVector asteroid2OverlapVector = multiplyVector(distanceUnitVector, getAsteroid(j)->getCollisionRadius());
					PositionVector overlapVector = subtractVectors(subtractVectors(distanceVector, asteroid1OverlapVector), asteroid2OverlapVector);
					PositionVector displacementVector = multiplyVector(overlapVector, 0.5);
					asteroid->setPosition(subtractVectors(asteroid->getPosition(), displacementVector));
					getAsteroid(j)->setPosition(addVectors(getAsteroid(j)->getPosition(), displacementVector));
				}
			}
		}

		// Check for collisions with bullets
		for (int j = 0; j < getBulletCount(); ++j) {
			if (isColliding(getBullet(j)->getPosition(), asteroid->getPosition(), asteroid->getCollisionRadius())) {
				// despawn bullet
				freeBullet(j);

				// remove health from asteroid
				asteroid->incrementHp(-PLAYER_DAMAGE_DEALT);
				if (asteroid->getHp() <= 0) {
					explodeAsteroid(i);
				}
			}
		}
	}
}

// Asteroid Physics
void explodeAsteroid(int index) {
	// Spawn smaller asteroids if size > 1
	Asteroid* asteroid = getAsteroid(index);
	if (asteroid) {
		if (asteroid->getSize() > 1) {
			splitAsteroid(asteroid);
		}

		// Create asteroid puff effect
		for (int i = 0; i < ASTEROID_PUFF_COUNT; ++i) {
			createParticle(asteroid->getPosition(), multiplyVector(angleToUnitVector((rand() % 100 / 100) * 2 * PI), 0.1f));
		}

		// Despawn Asteroid
		freeAsteroid(index);
	}
}

void splitAsteroid(Asteroid* asteroid) {
	// If there is room for more asteroids then split new asteroids
	for (int i = 0; getAsteroidCount() < MAX_ASTEROIDS && i < 2; ++i) {
		// Spawn new split asteroid
		pushAsteroid(new Asteroid(asteroid->getPosition().x + (1 - i * 2) * ASTEROID_SPLIT_RADIUS_MULTIPLIER, asteroid->getPosition().y));
		getAsteroid(getAsteroidCount() - 1)->setMoveVector(rotateVector(asteroid->getMoveVector(), ASTEROID_SPLIT_ANGLE, (-1 + i * 2)));
		getAsteroid(getAsteroidCount() - 1)->setSize(asteroid->getSize() - 1);
	}
}
