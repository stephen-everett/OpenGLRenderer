#pragma once
#include "Object3D.h"
#include "Animation.h"
class TranslationAnimation : public Animation {
private:
	glm::vec3 m_translation;

	void applyAnimation(float_t dt) override {
		object().move(m_translation * dt);
	}
public:
	TranslationAnimation(Object3D& object, float_t duration, 
		const glm::vec3& totalMovement) :
		Animation(object, duration), m_translation(totalMovement / duration) {}
};
