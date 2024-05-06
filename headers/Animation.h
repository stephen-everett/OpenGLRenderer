#pragma once
#include "Object3D.h"

/**
* @brief Represents an abstract animation of an object, manipulating one or more of its
* attributes over a duration.
* This is an abstract class that cannot be instantiated.
*/
class Animation {
private:
	float_t m_duration;
	float_t m_currentTime;
	std::shared_ptr<Object3D> m_object;

	/**
	 * @brief Called when the animation is activated by an Animator.
	 */
	virtual void startAnimation() {}
	/**
	 * @brief Called when the animation is ticked by an Animator.
	 * @param dt the change in time since the last tick.
	 */
	virtual void applyAnimation(float_t dt) = 0;

public:
	Animation(std::shared_ptr <Object3D> obj, float_t duration) : m_object(obj), m_duration(duration),
		m_currentTime(-1) {
	}

	/**
	* @brief The duration over which the animation is active.
	*/
	float_t duration() const { return m_duration; }

	/**
	 * @brief How much time has elapsed during the execution of this animation.
	 */
	float_t currentTime() const { return m_currentTime; }

	/**
	* @brief The object the animation is manipulating.
	*/
	std::shared_ptr<Object3D> object() const { return m_object; }

	/**
	* @brief Advances the animation by the given interval, in seconds.
	*/
	void tick(float_t dt) {
		m_currentTime += dt;
		applyAnimation(dt);
	}

	/**
	 * @brief Starts the animation.
	 */
	void start() {
		m_currentTime = 0;
		startAnimation();
	}

};
