#pragma once
#include <glm/glm.hpp>


// this light contains all values for global phong lighting
struct GlobalPhong {

	glm::vec3 viewPos;
	glm::vec3 ambientColor;
	glm::vec3 direction;
	glm::vec3 directionalColor;
	glm::vec3 material;

	GlobalPhong(glm::vec3 pos, glm::vec3 color, glm::vec3 dir, glm::vec3 dcolor, glm::vec3 mat) {
		viewPos = pos;
		ambientColor = color;
		direction = dir;
		directionalColor = dcolor;
		material = mat;
	}
};