#pragma once
#include "Object3D.h"
#include "Animation.h"
#include <cmath>

class ArcAnimation : public Animation {
private:
    float m_radius;
    float m_startAngle;
    float m_endAngle;
    float m_y;

    void applyAnimation(float_t dt) override {
        float currentAngle = m_startAngle + (m_endAngle - m_startAngle) * (currentTime() / duration());
        float x = m_radius * std::cos(currentAngle);
        float z = m_radius * std::sin(currentAngle);
        object()->setPosition(object()->getPosition() + glm::vec3(x, m_y, z));
    }

public:
    ArcAnimation(std::shared_ptr<Object3D> object, float_t duration, float startAngle, float endAngle, float radius, float y) :
        Animation(object, duration), m_startAngle(startAngle), m_endAngle(endAngle), m_radius(radius), m_y(y) {}
};
