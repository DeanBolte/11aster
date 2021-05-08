/*
*	physics contains functionality for movement and collision of objects
*
*	Main Contributer: Dean
*/

#include "physics.h"

//		Physics Engine Calls
void physicsMovements(float delta) {
	// Apply gravity from black holes
	// Applies to Asteroids, Bullets, particles and the Player
	//applyGravity(delta);

	// Update asteroid positions
	for (int i = 0; i < getAsteroidCount(); ++i) {
		getAsteroid(i)->update(delta);
	}

	// Update player
	getPlayer()->update(delta);

	// Update bullet positions
	moveBullets(delta);

	// Update particle positions
	moveParticles(delta);
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

// This is the heaviest part of the code base, its essentially the entire game, it could be heavily modified to be made faster
// however that is out of the scope of this project, i will however attempt to do as much optimisation in the time i have available
void asteroidCollisions() {
	for (int i = 0; i < getAsteroidCount(); ++i) {
		Asteroid* asteroid = getAsteroid(i);

		// Check for collision circle overlap for player
		Player* player = getPlayer();
		if (isColliding(player->getPosition(), asteroid->getPosition(), player->getCollisionRadius() + asteroid->getCollisionRadius())) {
			player->incrementHp(PLAYER_DAMAGE_TAKEN);
		}

		// Check for collisions with other asteroids
		for (int j = 0; j < getAsteroidCount(); ++j) {
			if (i != j) {
				// Distance between asteroids
				PositionVector distanceVector = subtractVectors(getAsteroid(i)->getPosition(), getAsteroid(j)->getPosition());
				float distance = vectorLength(distanceVector);
				if (distance < getAsteroid(i)->collisionRadius + getAsteroid(j)->collisionRadius) {
					PositionVector temp;
					temp.x = getAsteroid(i)->moveVector.x;
					temp.y = getAsteroid(i)->moveVector.y;
					getAsteroid(i)->moveVector.x = getAsteroid(j)->moveVector.x;
					getAsteroid(i)->moveVector.y = getAsteroid(j)->moveVector.y;
					getAsteroid(j)->moveVector.x = temp.x;
					getAsteroid(j)->moveVector.y = temp.y;

					// Move the asteroids apart so they arent within each other
					// the distance of overlap between the two asteroids is equal to the distance between there centres minus the non overlapping distance
					// here i calculate the vector of overlap and divide it in two to move the asteroids apart equally
					// this is to prevent asteroid overlap
					PositionVector distanceUnitVector = vectorToUnitVector(distanceVector);
					PositionVector asteroid1OverlapVector = multiplyVector(distanceUnitVector, getAsteroid(i)->collisionRadius);
					PositionVector asteroid2OverlapVector = multiplyVector(distanceUnitVector, getAsteroid(j)->collisionRadius);
					PositionVector overlapVector = subtractVectors(subtractVectors(distanceVector, asteroid1OverlapVector), asteroid2OverlapVector);
					PositionVector displacementVector = multiplyVector(overlapVector, 0.5);
					getAsteroid(i)->position = subtractVectors(getAsteroid(i)->position, displacementVector);
					getAsteroid(j)->position = addVectors(getAsteroid(j)->position, displacementVector);
				}
			}
		}

		// Check for collisions with bullets
		for (int j = 0; j < getBulletCount(); ++j) {
			float distance = vectorLength(subtractVectors(getBullet(j)->position, asteroid->position));

			if (distance < asteroid->collisionRadius) {
				// despawn bullet
				freeBullet(j);

				// remove health from asteroid
				asteroid->hp -= PLAYER_DAMAGE_DEALT;
				if (asteroid->hp <= 0) {
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
		if (asteroid->size > 1) {
			splitAsteroid(asteroid);
		}

		// Create asteroid puff effect
		for (int i = 0; i < ASTEROID_PUFF_COUNT; ++i) {
			createParticle(asteroid->position, multiplyVector(angleToUnitVector((rand() % 100 / 100) * 2 * PI), 0.1), ASTEROID_PUFF_SIZE);
		}

		// Despawn Asteroid
		freeAsteroid(index);
	}
}

void splitAsteroid(Asteroid* asteroid) {
	// If there is room for more asteroids then split new asteroids
	for (int i = 0; getAsteroidCount() < MAX_ASTEROIDS && i < 2; ++i) {
		// Spawn new split asteroid
		pushAsteroid(new Asteroid(asteroid->position.x + (1 - i * 2) * ASTEROID_SPLIT_RADIUS_MULTIPLIER, asteroid->position.y));
		getAsteroid(getAsteroidCount() - 1)->moveVector = rotateVector(asteroid->moveVector, ASTEROID_SPLIT_ANGLE, (-1 + i * 2));
		getAsteroid(getAsteroidCount() - 1)->size = asteroid->size - 1;
	}
}

// Movement
PositionVector movePosition(float delta, PositionVector position, PositionVector movement) {
	movement.x *= delta;
	movement.y *= delta;
	return addVectors(position, movement);
}

// Bullet Movement
void moveBullets(float delta) {
	for (int i = 0; i < getBulletCount(); ++i) {
		Bullet* bullet = getBullet(i);
		bullet->position = movePosition(delta, bullet->position, bullet->moveVector);
		// Theres an issue here, cullBullet removes the bullet struct before movebullets is complete, potentially skipping a bullet
		// or worse, potentially causing undefined behaviour
	}
}

// Engine Particle Movement
void moveParticles(float delta) {
	for (int i = 0; i < getParticleCount(); ++i) {
		// Move particle
		Particle* particle = getParticle(i);
		particle->position = movePosition(delta, particle->position, particle->moveVector);

		// Spin particle
		particle->angle += particle->spin * delta;
		if (particle->angle >= 2 * PI) {
			particle->angle -= 2 * PI;
		}
		else if (particle->angle <= 0) {
			particle->angle += 2 * PI;
		}

		// Theres an issue here, cullEngineParticle removes the engine particle struct before movebullets is complete, potentially skipping a bullet
		// or worse, potentially causing undefined behaviour
		// Decrease size
		particle->size -= particle->decaySpeed * delta;
		if (particle->size <= 0) {
			freeParticle(i);
		}
	}
}
