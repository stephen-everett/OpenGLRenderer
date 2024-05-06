#pragma once
#include "Object3D.h"
#include "Animation.h"
/**
 * @brief Translates an object at a continuous rate over an interval.
 */
class TranslationAnimation : public Animation {
private:
	/**
	 * @brief How much to increment the orientation by each second.
	 */
	glm::vec3 m_perSecond;

	/**
	 * @brief Advance the animation by the given time interval.
	 */
	void applyAnimation(float_t dt) override {
		// Moves the object based on the change of time
		// object().move(vec3 * float)
		object()->move(m_perSecond * dt);
	}

public:
	/**
	 * @brief Constructs a animation of a constant translation by the given total move
	 * linearly interpolated across the given duration.
	 */
	TranslationAnimation(std::shared_ptr<Object3D> object, float_t duration, const glm::vec3& totalTranslation) :
		Animation(object, duration), m_perSecond(totalTranslation / duration) {}
};