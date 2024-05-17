#pragma once
#include <glm/ext.hpp>
struct PhysicsStructure {
	float mass;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	PhysicsStructure(float m, glm::vec3 a, glm::vec3 v) {
		mass = m;
		acceleration = a;
		velocity = v;
	}

	PhysicsStructure() {
		mass = 0;
		acceleration = glm::vec3(0);
		velocity = glm::vec3(0);
	}
};