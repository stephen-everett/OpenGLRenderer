#pragma once
#include <memory>
#include "Animation.h"
#include "RotationAnimation.h"

class Animator {
private:
	/**
	 * @brief How much time has elapsed since the animation started.
	 */
	float_t m_currentTime;
	/**
	 * @brief The time at which we transition to the next animation.
	 */
	float_t m_nextTransition;
	/**
	 * @brief The sequence of animations to play.
	 */
	std::vector<std::unique_ptr<Animation>> m_animations;
	/**
	 * @brief The current (active) animation.
	 */
	Animation* m_currentAnimation;
	/**
	 * @brief The index of the current animation.
	 */
	int32_t m_currentIndex;
	
	/**
	 * @brief Activate the next animation.
	 */
	void nextAnimation();

public:
	/**
	 * @brief Constructs an Animator that acts on the given object.
	 */
	Animator() :
		m_currentTime(0),
		m_nextTransition(0),
		m_currentIndex(-1), 
		m_currentAnimation(nullptr) {
	}

	/**
	 * @brief Add an Animation to the end of the animation sequence.
	 */
	void addAnimation(std::unique_ptr<Animation> animation) {
		m_animations.emplace_back(std::move(animation));
	}

	/**
	 * @brief Activate the Animator, causing its active animation to receive future tick() calls.
	 */
	void start();

	/**
	 * @brief Advance the animation sequence by the given time interval, in seconds.
	 */
	void tick(float_t dt);

};