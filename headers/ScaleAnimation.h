#pragma once
#include "Object3D.h"
#include "Animation.h"

class ScaleAnimation : public Animation {
private:
    glm::vec3 m_scale;

    void applyAnimation(float_t dt) override {
        // Set the scale directly
        object()->setScale(m_scale);
    }

public:
    ScaleAnimation(std::shared_ptr<Object3D> object, float_t duration, const glm::vec3& scale) :
        Animation(object, duration), m_scale(scale) {}
};