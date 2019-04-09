#pragma once
#include <glm/glm.hpp>
#include "Boid.h"

class BoidSystem {
public:
	BoidSystem();
	~BoidSystem();
	void RunSystem(float deltaTime, glm::mat4 viewMatrix, glm::mat4 projMatrix);
	void DisplayBoids();

	std::vector<Boid*> boids;

private:
	float timer;
	float spawnTime = 2.0f;

};