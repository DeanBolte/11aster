/*	
*	elements contains functionality for object data storage and rendering
* 
*	Main Author: Dean
*/

#include "elements.h"

// Object Data
Asteroid* asteroidArray[MAX_ASTEROIDS];
int asteroidCount = 0;

Bullet* bulletArray[MAX_BULLETS];
int bulletCount = 0;

Player* playerData = NULL;

Particle* particleArray[MAX_ENGINE_PARTICLES];
int particleCount = 0;

BlackHole* blackHoleArray[MAX_BLACKHOLES];
int blackHoleCount;

// Object Initialisation
Asteroid* initAsteroid(float x, float y) {
	// Create and return the asteroid struct where requested
	Asteroid* asteroid = NULL;
	while (asteroid == NULL) {
		asteroid = malloc(sizeof(Asteroid));
	}
	
	// initialise variables
	asteroid->position.x = x;
	asteroid->position.y = y;
	asteroid->moveVector.x = 0; 
	asteroid->moveVector.y = 0;
	asteroid->inside = 0;
	asteroid->vertexCount = ASTEROID_VERTEX_COUNT;
	asteroid->size = rand() % 2 + 1;
	asteroid->hp = asteroid->size * ASTEROID_HP_MULTIPLIER;
	asteroid->angle = PI/2;
	asteroid->spin = rand() % ASTEROID_SPIN - ASTEROID_SPIN / 2;

	// Collision
	asteroid->collisionRadius = asteroid->size + ASTEROID_BASE_SIZE + ASTEROID_VARIANCE * asteroid->size / 2;

	// Create vertices
	asteroid->vertices = malloc(sizeof(PositionVector) * asteroid->vertexCount);
	while (asteroid->vertices == NULL) {
		asteroid->vertices = malloc(sizeof(PositionVector) * asteroid->vertexCount);
	}

	PositionVector vertex;
	vertex.x = rand() % ASTEROID_VARIANCE - ASTEROID_VARIANCE / 2;
	vertex.y = rand() % ASTEROID_VARIANCE + ASTEROID_BASE_SIZE;
	for (int i = 0; i < asteroid->vertexCount; ++i) {
		asteroid->vertices[i] = vertex;
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

		vertex = rotateVector(vertex, (2 * PI) / asteroid->vertexCount, 1);
	}
	
	return asteroid;
}

Player* initPlayer(float x, float y) {
	Player* player = NULL;
	while (player == NULL) {
		player = malloc(sizeof(Player));
	}

	player->position.x = x;
	player->position.y = y;
	player->moveVector.x = 0;
	player->moveVector.y = 0;
	player->direction.x = 0.866;
	player->direction.y = 0.5;
	player->firing = 0;
	// Cooldowns in seconds
	player->cannonCoolDown = 0;
	player->particleCoolDown = 0;
	player->collisionRadius = PLAYER_COLLISION_RADIUS;
	player->acceleration = PLAYER_ACCELERATION;
	player->maxVelocity = PLAYER_MAXIMUM_VELOCITY;
	player->minVelocity = PLAYER_MINIMUM_VELOCITY;
	return player;
}

Bullet* initBullet(Player* player) {
	Bullet* bullet = NULL;
	while (bullet == NULL) {
		bullet = malloc(sizeof(Bullet));
	}
	
	bullet->position.x = player->position.x;
	bullet->position.y = player->position.y;
	bullet->moveVector.x = player->direction.x * BULLET_VECOLITY;
	bullet->moveVector.y = player->direction.y * BULLET_VECOLITY;
	return bullet;
}

Particle* initParticle(PositionVector position, PositionVector direction, int maxSize) {
	Particle* particle = NULL;
	while (particle == NULL) {
		particle = malloc(sizeof(Particle));
	}

	// positon = player position + randomness - player direction
	// player direction is subtracted to spawn particles at the back of the ship
	particle->position.x = position.x + (rand() % 10 - 5) - direction.x * 10;
	particle->position.y = position.y + (rand() % 10 - 5) - direction.y * 10;

	// velocity is based on the inverse direction of acceleration of the player + some randomness
	particle->moveVector.x = -direction.x * 1000 + rand() % 200;
	particle->moveVector.y = -direction.y * 1000 + rand() % 200;
	particle->decaySpeed = rand() % (PARTICLE_DECAY_SPEED / 2) + PARTICLE_DECAY_SPEED;
	particle->size = rand() % maxSize;
	particle->spin = rand() % 5 - 2.5;
	particle->angle = PI / 2;
	return particle;
}

BlackHole* initBlackHole(PositionVector position) {
	BlackHole* bh = NULL;
	while (bh == NULL) {
		bh = malloc(sizeof(BlackHole));
	}

	bh->position = position;
	bh->radius = rand() % (int)(BLACKHOLE_RADIUS / 2) + BLACKHOLE_RADIUS;
	bh->radiusOutPulse = bh->radius + rand() % (int)(BLACKHOLE_RADIUS / 2);
	bh->radiusInPulse = bh->radius - rand() % (int)(BLACKHOLE_RADIUS / 2);
	bh->gravity = bh->radius * GRAVITY_MULTIPLIER;
	bh->pulseDirection = 1;
	return bh;
}

// Object Creation
void createAsteroid(float x, float y) {
	if (asteroidCount < MAX_ASTEROIDS) {
		Asteroid* asteroid = initAsteroid(x, y);
		asteroidArray[asteroidCount] = asteroid;
		++asteroidCount;
	}
}

void createPlayer(float x, float y) {
	if (playerData != NULL) {
		destructPlayer(playerData);
	}
	playerData = initPlayer(x, y);
}

void createParticle(PositionVector position, PositionVector velocity, int maxSize) {
	if (particleCount < MAX_ENGINE_PARTICLES) {
		Particle* particle = initParticle(position, velocity, maxSize);
		particleArray[particleCount] = particle;
		++particleCount;
	}
}

void createBlackHole(PositionVector position) {
	if (blackHoleCount < MAX_BLACKHOLES) {
		blackHoleArray[blackHoleCount] = initBlackHole(position);
		++blackHoleCount;
	}
}

// Object Access
Player* getPlayer() {
	return playerData;
}

Asteroid* getAsteroid(int index) {
	return asteroidArray[index];
}

int getAsteroidCount() {
	return asteroidCount;
}

Particle* getParticle(int index) {
	return particleArray[index];
}

int getParticleCount() {
	return particleCount;
}

Bullet* getBullet(int index) {
	return bulletArray[index];
}

int getBulletCount() {
	return bulletCount;
}

BlackHole* getBlackHole(int index) {
	return blackHoleArray[index];
}

int getBlackHoleCount() {
	return blackHoleCount;
}

// Object Destruction
void freeAllObjects() {
	// Free all game objects
	freeAsteroids();
	freeBullets();
	freeParticles();
	freeBlackHoles();

	// Free the player
	if (playerData) {
		destructPlayer(playerData);
	}
}

void freeAsteroids() {
	// Free asteroids
	for (int i = 0; i < asteroidCount; ++i) {
		destructAsteroid(asteroidArray[i]);
	}
	asteroidCount = 0;
}

void freeBullets() {
	// Free bullets
	for (int i = 0; i < bulletCount; ++i) {
		destructBullet(bulletArray[i]);
	}
	bulletCount = 0;
}

void freeParticles() {
	// Free particles
	for (int i = 0; i < particleCount; ++i) {
		destructParticle(particleArray[i]);
	}
	particleCount = 0;
}

void freeBlackHoles() {
	// Free blackholes
	for (int i = 0; i < blackHoleCount; ++i) {
		destructBlackHole(blackHoleArray[i]);
	}
	blackHoleCount = 0;
}

void freeAsteroid(int index) {
	if (index > 0 && index < asteroidCount) {
		Asteroid* asteroid = asteroidArray[index];

		// Shuffle array
		for (int i = index; i < asteroidCount - 1; ++i) {
			asteroidArray[i] = asteroidArray[i + 1];
		}
		--asteroidCount;

		// Free asteroid
		destructAsteroid(asteroid);
	}
}

void freeBullet(int index) {
	if (index > 0 && index < bulletCount) {
		Bullet* bullet = bulletArray[index];

		// Shuffle array
		for (int i = index; i < bulletCount - 1; ++i) {
			bulletArray[i] = bulletArray[i + 1];
		}
		--bulletCount;

		// Free bullet
		destructBullet(bullet);
	}
}

void freeParticle(int index) {
	if (index > 0 && index < particleCount) {
		Particle* particle = particleArray[index];

		// Shuffle array
		for (int i = index; i < particleCount - 1; ++i) {
			particleArray[i] = particleArray[i + 1];
		}
		--particleCount;

		// Free particle
		destructParticle(particle);
	}
}

void freeBlackHole(int index) {
	if (index > 0 && index < blackHoleCount) {
		BlackHole* bh = blackHoleArray[index];

		// Shuffle array
		for (int i = index; i < blackHoleCount - 1; ++i) {
			blackHoleArray[i] = blackHoleArray[i + 1];
		}
		--blackHoleCount;

		// Free particle
		destructBlackHole(bh);
	}
}

void destructAsteroid(Asteroid* asteroid) {
	free(asteroid->vertices);
	free(asteroid);
}

void destructPlayer(Player* player) {
	free(player);
}

void destructBullet(Bullet* bullet) {
	free(bullet);
}

void destructParticle(Particle* particle) {
	free(particle);
}

void destructBlackHole(BlackHole* bh) {
	free(bh);
}

// Object Rendering
void renderAsteroids() {
	// Create the asteroid in OpenGl
	for (int i = 0; i < asteroidCount; ++i) {
		float x = asteroidArray[i]->position.x;
		float y = asteroidArray[i]->position.y;
		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((asteroidArray[i]->angle * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawAsteroid(asteroidArray[i]->vertices, asteroidArray[i]->vertexCount, asteroidArray[i]->size);

		glPopMatrix();
	}
}

void renderPlayer() {
	if (playerData != NULL) {
		float x = playerData->position.x;
		float y = playerData->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((vectorAngle(playerData->direction) * 180.0 / PI) - 90, 0.0, 0.0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawPlayer();

		glPopMatrix();
	}
}

void renderBullets() {
	for (int i = 0; i < bulletCount; ++i) {
		float x = bulletArray[i]->position.x;
		float y = bulletArray[i]->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawBullet();

		glPopMatrix();
	}
}

void renderEngineParticles() {
	for (int i = 0; i < particleCount; ++i) {
		float x = particleArray[i]->position.x;
		float y = particleArray[i]->position.y;

		glPushMatrix();
		glTranslatef(x, y, 1.0);
		glRotatef((particleArray[i]->angle * 180.0 / PI) - 90, 0, 0, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawPentagon(particleArray[i]->size);

		glPopMatrix();
	}
}

void renderBlackHoles() {
	for (int i = 0; i < blackHoleCount; ++i) {
		glPushMatrix();
		glTranslatef(blackHoleArray[i]->position.x, blackHoleArray[i]->position.y, 1.0);

		glColor3f(highColour.r, highColour.g, highColour.b);

		drawBlackHole(blackHoleArray[i]);

		glPopMatrix();
	}
}

// Object Rendering
void drawAsteroid(PositionVector* vertices, int vertexCount, int size) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexCount; ++i) {
		glVertex2f(vertices[i].x * size, vertices[i].y * size);
	}
	glEnd();
}

void drawPlayer() {
	glBegin(GL_TRIANGLES);
	glVertex2f(-20, -20);
	glVertex2f(0, 20);
	glVertex2f(0, -10);
	glVertex2f(0, 20);
	glVertex2f(20, -20);
	glVertex2f(0, -10);
	glEnd();
}

void drawBullet() {
	drawPentagon(3);
}

void drawBlackHole(BlackHole* bh) {
	drawCircle(bh->radius);
}

// Rendering Shapes
void drawPentagon(float radius) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 1 * radius);
	glVertex2f(0.9511 * radius, 0.309 * radius);
	glVertex2f(0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.5878 * radius, -0.809 * radius);
	glVertex2f(-0.9511 * radius, 0.309 * radius);
	glEnd();
}

void drawCircle(float radius) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; ++i) {
		glVertex2f(cos(i * PI / 180) * radius, sin(i * PI / 180) * radius);
	}
	glEnd();
}

// Rendering Other
void drawText(float x, float y, const char* text, int length, float size) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(size, size, 1.0);

	for (int i = 0; i < length; ++i) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	glPopMatrix();
}