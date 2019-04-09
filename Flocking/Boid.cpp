#include "Boid.h"
#include <glm/glm.hpp>

Boid::Boid() {
	object = new GameObject();
	shape = new Cube();
	(*shape).Load();
	(*object).model = shape;

	(*shape).fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	(*object).scale = glm::vec3(0.5f, 0.5f, 0.5f);
	(*object).position = glm::vec3(0.0f, 1.5f, 0.0f);
}

Boid::~Boid() {
	delete object;
	delete shape;
}


