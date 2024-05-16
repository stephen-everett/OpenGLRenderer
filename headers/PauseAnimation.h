#pragma once
#include "Object3D.h"
#include "Animation.h"
#include <glm/glm.hpp>

/**
 * @brief Pauses the animation for a given duration.
 */
class PauseAnimation : public Animation {
private:
	/**
	 * @brief Advance the animation by the given time interval.
	 */
	void applyAnimation(float_t dt) override {
		// Do nothing
	}

public:
	/**
	 * @brief Constructs an animation that pauses for a given duration.
	 */
	PauseAnimation(std::shared_ptr<Object3D> object, float_t duration) : Animation(object, duration) {}
};