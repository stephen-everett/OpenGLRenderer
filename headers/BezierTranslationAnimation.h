#pragma once
#include "Object3D.h"
#include "Animation.h"
#include <glm/glm.hpp>
#include <iostream>

/**
 * @brief Animates an object along a cubic Bezier curve over a given duration.
 */
class BezierTranslationAnimation : public Animation {
private:
    glm::vec3 p0; // First control point
    glm::vec3 p1; // Second control point
    glm::vec3 p2; // Third control point
    glm::vec3 p3; // Fourth control point
    glm::vec3 initialPosition; // Stores the object's initial position at the start of the animation
    bool firstUpdate; // Flag to check if it's the first update call

    /**
     * @brief Advance the animation by the given time interval.
     * @param dt Delta time - time elapsed since the last update.
     */
    void applyAnimation(float_t dt) override {
        if (firstUpdate) {
            initialPosition = object()->getPosition(); // Capture the initial position at the start
            std::cout << "Initial Position: " << initialPosition.x << ", " << initialPosition.y << ", " << initialPosition.z << std::endl;
            firstUpdate = false;
        }

        float_t t = currentTime() / duration(); // normalized time: 0 to 1
        glm::vec3 newPosition =
            (1 - t) * (1 - t) * (1 - t) * p0 +
            3 * (1 - t) * (1 - t) * t * p1 +
            3 * (1 - t) * t * t * p2 +
            t * t * t * p3;

        glm::vec3 bezierPoint = initialPosition + newPosition; // Calculate absolute bezier point
        glm::vec3 deltaPosition = bezierPoint - object()->getPosition(); // Calculate the delta to move

        // Move the object by the delta position
        object()->move(deltaPosition);
        
        // For Testing
        //std::cout << "Delta Position: " << deltaPosition.x << ", " << deltaPosition.y << ", " << deltaPosition.z << std::endl;
    }

public:
    /**
     * @brief Constructs an animation that moves an object along a cubic Bezier curve defined by four control points.
     * @param object Reference to the object to animate.
     * @param duration Duration of the animation.
     * @param controlPoint0 First control point of the Bezier curve.
     * @param controlPoint1 Second control point of the Bezier curve.
     * @param controlPoint2 Third control point of the Bezier curve.
     * @param controlPoint3 Fourth control point of the Bezier curve.
     */
    BezierTranslationAnimation(std::shared_ptr<Object3D> object, float_t duration, const glm::vec3& controlPoint0,
        const glm::vec3& controlPoint1, const glm::vec3& controlPoint2,
        const glm::vec3& controlPoint3) :
        Animation(object, duration), p0(controlPoint0), p1(controlPoint1), p2(controlPoint2), p3(controlPoint3),
        firstUpdate(true) {}
};
