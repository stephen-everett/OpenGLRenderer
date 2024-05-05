	#include "Animator.h"

void Animator::nextAnimation() {
	// Increase the animation index, and start the next animation if there is one.
	++m_currentIndex;

	if (m_currentIndex < m_animations.size()) {
		m_currentAnimation = m_animations[m_currentIndex].get();
		m_currentAnimation->start();
		m_nextTransition = m_nextTransition + m_currentAnimation->duration();
	}
	else {
		m_currentIndex = -1;
		m_currentAnimation = nullptr;
	}
}

void Animator::tick(float_t dt) {
	// Advance the active animation by the given interval.
	if (m_currentIndex >= 0) {
		float_t lastTime = m_currentTime;
		m_currentTime += dt;

		// If our current time surpasses the next transition time, we need to tick
		// both the active animation (up to the transition time), and the subsequent animation
		// (by the amount we exceeded the transition time).
		if (m_currentTime >= m_nextTransition) {
			m_currentAnimation->tick(m_nextTransition - lastTime);
			float_t overTime = m_currentTime - m_nextTransition;
			nextAnimation();
			if (m_currentAnimation != nullptr) {
				m_currentAnimation->tick(overTime);
			}
		}
		else {
			m_currentAnimation->tick(dt);
		}
	}
}

void Animator::start() {
	m_currentTime = 0;
	nextAnimation();
}
