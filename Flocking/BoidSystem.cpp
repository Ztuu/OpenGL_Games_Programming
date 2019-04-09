#include "BoidSystem.h"
#include "Boid.h"
#include <glm/glm.hpp>
#include <cstdlib>

BoidSystem::BoidSystem() {

}

BoidSystem::~BoidSystem() {
	for (int i = 0; i < boids.size(); i++) {
		delete boids[i];
	}
}

//This is called every frame by the main loop
void BoidSystem::RunSystem(float deltaTime, glm::mat4 viewMatrix, glm::mat4 projMatrix) {
	
	//Gradually spawn in boids until we have the desired number
	if (boids.size() < 40) {
		if (timer <= 0.0f) {
			boids.push_back(new Boid());
			timer = spawnTime;
		}
		else {
			timer -= deltaTime;
		}
		
	}
	
	//Calculate force for each boid
	//Force consist of direction, position, cohesion and boundary components
	for (int i = 0; i < boids.size(); i++) {
		glm::vec3 averageDirection = glm::vec3(0.0f);
		glm::vec3 averagePosition = (*(*boids[i]).object).position;
		glm::vec3 cohesionForce = glm::vec3(0.0f);
		int counter = 1;

		//Look for any other boids close to this one
		for (int k = 0; k < boids.size(); k++) {
			if (i == k) {
				//ignore self
				continue;
			}
			else {
				float distance = glm::distance((*(*boids[i]).object).position, (*(*boids[k]).object).position);
				
				//Sum direction and position for any neighbouring boids within a certain distance
				if (distance < 3.0f) {
					
					averageDirection += (*boids[k]).targetDirection;
					averagePosition += (*(*boids[k]).object).position;
					counter++;
					
					//If boids are too close, cohesion force is added to push them apart
					if (distance < 1.0f) {
						if ((*(*boids[k]).object).position == (*(*boids[i]).object).position) {
							cohesionForce += glm::vec3(1.0f, 0.0f, 0.0f);
						}
						else {
							cohesionForce += 2.5f * ( (*(*boids[i]).object).position - (*(*boids[k]).object).position);
						}
					}
				}				
			}
		}
		
		//Quick fix to steer boids back in if near boundary because there are no bounds
		glm::vec3 boundaryForce = glm::vec3(0.0f);
		if ((*(*boids[i]).object).position.x > 25.0f) {
			boundaryForce.x = 25.0f - (*(*boids[i]).object).position.x;
		}else if((*(*boids[i]).object).position.x < -25.0f) {
			boundaryForce.x = -25.0f - (*(*boids[i]).object).position.x;
		}
		if ((*(*boids[i]).object).position.y > 7.5f && (*(*boids[i]).object).velocity.y > -2.0f) {
			boundaryForce.y = 7.5f - (*(*boids[i]).object).position.y;
		}
		else if ((*(*boids[i]).object).position.y < 6.5f && (*(*boids[i]).object).velocity.y < 2.0f) {
			boundaryForce.y = 6.5f - (*(*boids[i]).object).position.y;
		}
		if ((*(*boids[i]).object).position.z > 25.0f) {
			boundaryForce.z = 25.0f - (*(*boids[i]).object).position.z;
		}
		else if ((*(*boids[i]).object).position.z < -25.0f) {
			boundaryForce.z = -25.0f - (*(*boids[i]).object).position.z;
		}
		
		//Calculate average forces
		glm::vec3 positionForce = 0.5f * ((1.0f / counter) * averagePosition - (*(*boids[i]).object).position);
		if (counter > 1) {
			//Update direction to new average
			averageDirection = glm::normalize(averageDirection);
			(*boids[i]).targetDirection = averageDirection;
		}
		
		//Calculate final direction force by weighting each component
		(*boids[i]).targetDirection = 1.0f * (*boids[i]).targetDirection + 0.25f * positionForce + 4.0f * cohesionForce;
		(*boids[i]).targetDirection = glm::normalize((*boids[i]).targetDirection);
		(*(*boids[i]).object).forces = 4.0f * (*boids[i]).targetDirection + 1.0f * boundaryForce;
		
		//Call object methods to correctly display the boid
		(*(*boids[i]).object).UpdatePosition(deltaTime);
		(*(*boids[i]).object).CalculateMvMatrix(viewMatrix, projMatrix);
	}
}

//Draws the model associated with each boid
void BoidSystem::DisplayBoids() {
	for (int i = 0; i < boids.size(); i++) {
		(*(*(*boids[i]).object).model).Draw();
	}
}