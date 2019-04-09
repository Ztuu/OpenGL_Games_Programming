#pragma once

#include "GameObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

class Boid {

public:
	Boid();
	~Boid();

	glm::vec3 targetDirection = glm::vec3(-0.8f, 0.0f, -1.0f);

	GameObject* object;
	Cube* shape;
};