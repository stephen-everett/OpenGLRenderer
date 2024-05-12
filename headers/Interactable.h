#pragma once
#include <glm/ext.hpp>
#include <functional>
#include <memory>
#include <optional>

struct Interactable {
	const glm::vec3* position;
	std::function<void(bool)> interact_toggle;

	Interactable(const glm::vec3* pos, std::function<void(bool)> tog) {
		position = pos;
		interact_toggle = tog;
	}
};