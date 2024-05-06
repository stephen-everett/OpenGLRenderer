#pragma once
#include "Object3D.h"
#include "Animation.h"
/**
 * @brief Rotates an object at a continuous rate over an interval.
 */
class RotationAnimation : public Animation {
private:
	/**
	 * @brief How much to increment the orientation by each second.
	 */
	glm::vec3 m_perSecond;

	/**
	 * @brief Advance the animation by the given time interval.
	 */
	void applyAnimation(float_t dt) override {
		object()->rotate(m_perSecond * dt);
	}

public:
	/**
	 * @brief Constructs a animation of a constant rotation by the given total rotation 
	 * angle, linearly interpolated across the given duration.
	 */
	RotationAnimation(std::shared_ptr<Object3D> object, float_t duration, const glm::vec3& totalRotation) : 
		Animation(object, duration), m_perSecond(totalRotation / duration) {}
};

